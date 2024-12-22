#include "cpu.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

void loadFromFile(std::vector<uint32_t> &instructions)
{
	std::ifstream input;
	std::string fileName;
	uint32_t instr;
	std::cout << "Введите имя файла >> ";
	std::cin >> fileName;
	input.open(fileName);
	if (!input.is_open())
	{
		throw "Невозможно открыть файл";
	}
	while (!input.eof())
	{
		input >> std::hex >> instr;
		instructions.push_back(instr);
	}
	instructions.pop_back();
}

void saveToFile(const std::vector<uint32_t> &instructions)
{
	std::ofstream output;
	std::string fileName;
	std::cout << "Введите имя файла >> ";
	std::cin >> fileName;
	output.open(fileName);
	if (!output.is_open())
	{
		throw "Невозможно открыть файл";
	}
	for (auto i : instructions)
	{
		output << std::hex << i << '\n';
	}
}

uint32_t instructionBuilder()
{
	uint32_t ret = 0;
	int choice = 0;
	std::cout << "Выберите тип инструкции:\n"
	          << "1. R-type\n"
	          << "2. I-type\n"
	          << "3. S-type\n"
	          << "4. U-type (LUI)\n"
	          << "5. B-type\n"
	          << "6. J-type\n"
	          << ">> ";
	std::cin >> choice;
	switch (choice)
	{
	case 1: { // R-type
		uint16_t rd, rs1, rs2;
		ret += 0x00000033;
		std::cout << "Выберите инструкцию:\n"
		          << "1. ADD\n"
		          << "2. AND\n"
		          << "3. OR\n"
		          << "4. SRL\n"
		          << "5. SLL\n"
		          << "6. XOR\n"
		          << ">> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			break;
		case 2:
			ret += 0x00007000;
			break;
		case 3:
			ret += 0x00006000;
			break;
		case 4:
			ret += 0x00005000;
			break;
		case 5:
			ret += 0x00001000;
			break;
		case 6:
			ret += 0x00004000;
			break;
		default:
			throw "Invalid instruction";
		}
		std::cout << "Введите регистр назначения\n>>";
		std::cin >> rd;
		std::cout << "Введите первый регистр источник\n>>";
		std::cin >> rs1;
		std::cout << "Введите второй регистр источник\n>>";
		std::cin >> rs2;
		if (rd > 32 || rs1 > 32 || rs2 > 32)
		{
			throw "There is only 32 registers";
		}
		ret += (rd << 7) | (rs1 << 15) | (rs2 << 20);
		break;
	}
	case 2: { // I-type
		uint16_t imm, rd, rs1;
		ret += 0x00000013;
		std::cout << "Выберите инструкцию:\n"
		          << "1. ADDI\n"
		          << "2. ANDI\n"
		          << "3. ORI\n"
		          << "4. SRLI\n"
		          << "5. SLLI\n"
		          << "6. XORI\n"
		          << ">> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			break;
		case 2:
			ret += 0x00007000;
			break;
		case 3:
			ret += 0x00006000;
			break;
		case 4:
			ret += 0x00005000;
			break;
		case 5:
			ret += 0x00001000;
			break;
		case 6:
			ret += 0x00004000;
			break;
		default:
			throw "Invalid instruction";
		}
		std::cout << "Введите регистр назначения\n>>";
		std::cin >> rd;
		std::cout << "Введите первый регистр источник\n>>";
		std::cin >> rs1;
		std::cout << "Введите значение\n>>";
		std::cin >> imm;
		if (rd > 32 || rs1 > 32)
		{
			throw "There is only 32 registers";
		}
		ret += (rd << 7) | (rs1 << 15) | ((imm & 0xFFF) << 20);
		break;
	}
	case 3: { // S-type
		uint16_t rs1, rs2, imm;
		ret += 0x00000023;
		std::cout << "Выберите инструкцию:\n"
		          << "1. SB\n"
		          << "2. SH\n"
		          << "3. SW\n"
		          << ">> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			break;
		case 2:
			ret += 0x00001000;
			break;
		case 3:
			ret += 0x00002000;
			break;
		default:
			throw "Invalid instruction";
		}
		std::cout << "Введите сдвиг\n>>";
		std::cin >> imm;
		std::cout << "Введите первый регистр источник\n>>";
		std::cin >> rs1;
		std::cout << "Введите второй регистр источник\n>>";
		std::cin >> rs2;
		if (rs2 > 32 || rs1 > 32)
		{
			throw "There is only 32 registers";
		}
		ret += (rs2 << 20) | (rs1 << 15) | ((imm & 0x1F) << 7) | ((imm & 0x1E) << 8) | ((imm & 0xFE0) << 25);
		break;
	}
	case 4: { // U-type
		uint32_t imm;
		uint16_t rd;
		ret += 0x00000037;
		std::cout << "Введите регистр назначения\n>>";
		std::cin >> rd;
		std::cout << "Введите значение\n>>";
		std::cin >> imm;
		if (rd > 32)
		{
			throw "There is only 32 registers";
		}
		ret += (rd << 7) | ((imm & 0xFFFFF000) << 12);
		break;
	}
	case 5: { // B-type
		uint16_t rs1, rs2, imm;
		ret += 0x00000063;
		std::cout << "Выберите инструкцию:\n"
		          << "1. BEQ\n"
		          << "2. BNE\n"
		          << "3. BLT\n"
		          << "4. BLTU\n"
		          << "5. BGE\n"
		          << "6. BGEU\n"
		          << ">> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			break;
		case 2:
			ret += 0x00001000;
			break;
		case 3:
			ret += 0x00004000;
			break;
		case 4:
			ret += 0x00006000;
			break;
		case 5:
			ret += 0x00005000;
			break;
		case 6:
			ret += 0x00007000;
			break;
		default:
			throw "Invalid instruction";
		}
		std::cout << "Введите сдвиг\n>>";
		std::cin >> imm;
		std::cout << "Введите первый регистр источник\n>>";
		std::cin >> rs1;
		std::cout << "Введите второй регистр источник\n>>";
		std::cin >> rs2;
		if (rs2 > 32 || rs1 > 32)
		{
			throw "There is only 32 registers";
		}
		ret += (rs2 << 20) | (rs1 << 15) | ((imm & 0x800) << 7) | ((imm & 0x1E) << 8) | ((imm & 0x7E0) << 25) |
		       ((imm & 0x1000) << 31);
		break;
	}
	case 6: { // J-type
		ret += 0x0000006F;
		break;
	}
	default:
		throw "Unknown type of instruction";
	}
	return ret;
	/* TODO: complete the instruction generation*/
}

void menu()
{
	CPU cpu;
	std::vector<uint32_t> instructions;
	int choice = 0;
	while (true)
	{
		std::cout << "Выберите действие:\n"
		          << "1. Добавить инструкцию\n"
		          << "2. Удалить инструкцию\n"
		          << "3. Очистить список инструкций\n"
		          << "4. Запустить конвейер (на n тактов)\n"
		          << "5. Выполнить один такт конвейера\n"
		          << "6. Вывести состояние регистров\n"
		          << "7. Вывести состояние памяти\n"
		          << "8. Вывести список инструкций\n"
		          << "9. Очистить состояние процессора\n"
		          << "10. Загрузить инструкции из файла\n"
		          << "11. Сохранить инструкции в файле\n"
		          << "0. Выйти\n"
		          << ">> ";
		std::cin >> choice;
		if (!choice)
			break;
		switch (choice)
		{
		case 1: // add instruction
			try
			{
				instructions.push_back(instructionBuilder());
			}
			catch (const char *err)
			{
				std::cout << err << '\n';
			}
			break;
		case 2: // delete instruction
			std::cout << "Какую инструкцию вы хотите удалить?\n";
			for (int i = 0; i < instructions.size(); ++i)
			{
				std::cout << i << ". " << std::hex << instructions[i] << '\n';
			}
			std::cout << ">> ";
			std::cin >> choice;
			instructions.erase(instructions.begin() + choice);
			break;
		case 3: // clear instructions
			instructions.clear();
			break;
		case 4: // run pipeline for n ticks
			std::cout << "Введите количество тактов\n>> ";
			std::cin >> choice;
			cpu.pipeline(instructions, choice);
			break;
		case 5: // run pipeline for 1 tick
			cpu.pipeline(instructions);
			break;
		case 6: // print registers
			cpu.printRegs();
			break;
		case 7: // print memory
		{
			uint32_t begin, size;
			std::cout << "Введите адрес памяти >> ";
			std::cin >> begin;
			std::cout << "Введите количество байтов >> ";
			std::cin >> size;
			cpu.printMem(begin, size);
			break;
		}
		case 8: // print instructions
			for (int i = 0; i < instructions.size(); ++i)
			{
				std::cout << i << ": " << std::hex << instructions[i] << '\n';
			}
			break;
		case 9: // reset cpu
			cpu.reset();
			break;
		case 10: // load from file
			try
			{
				loadFromFile(instructions);
			}
			catch (const char *err)
			{
				std::cerr << err << '\n';
			}
			break;
		case 11: // save to file
			try
			{
				saveToFile(instructions);
			}
			catch (const char *err)
			{
				std::cerr << err << '\n';
			}
			break;
		default:
			std::cout << "Некорректный выбор\n";
		}
	}
	/*TODO: complete menu*/
}

int main()
{
	/* CPU cpu;
	std::vector<uint32_t> instructions = {0x00400313, 0x00500393, 0x33, 0x00638E33, 0x00100313, 0x33, 0x006E1E33};
	cpu.pipeline(instructions, 10);
	std::cout << "t1: " << cpu.getReg(6) << "\nt2: " << cpu.getReg(7) << "\nt3: " << cpu.getReg(28); */
	menu();
	return 0;
}
