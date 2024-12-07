#include "cpu.h"
#include <cstdint>
#include <iostream>
#include <vector>

uint32_t instructionBuilder()
{
	uint32_t ret = 0;
	int choice = 0;
	std::cout << "Выберите тип инструкции:\n"
	          << "1. R-type\n"
	          << "2. I-type\n"
	          << "3. S-type\n"
	          << "4. U-type\n"
	          << "5. B-type\n"
	          << "6. J-type\n"
	          << ">> ";
	std::cin >> choice;
	switch (choice)
	{
	case 1: {
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
	case 2: {
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
	case 3:
		ret += 0x00000023;
		break;
	case 4:
		ret += 0x00000037;
		break;
	case 5:
		ret += 0x00000063;
		break;
	case 6:
		ret += 0x0000006F;
		break;
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
		          << "4. Запустить конвейер\n"
		          << "5. Вывести список инструкций\n"
		          << "0. Выйти\n"
		          << ">> ";
		std::cin >> choice;
		if (!choice)
			break;
		switch (choice)
		{
		case 1:
			try
			{
				instructions.push_back(instructionBuilder());
			}
			catch (const char *err)
			{
				std::cout << err << '\n';
			}
			break;
		case 2:
			std::cout << "Какую инструкцию вы хотите удалить?\n";
			for (int i = 0; i < instructions.size(); ++i)
			{
				std::cout << i << ". " << std::hex << instructions[i] << '\n';
			}
			std::cout << ">> ";
			std::cin >> choice;
			instructions.erase(instructions.begin() + choice);
			break;
		case 3:
			instructions.clear();
			break;
		case 4:
			cpu.pipeline(instructions, instructions.size() + 5);
			break;
		case 5:
			for (int i = 0; i < instructions.size(); ++i)
			{
				std::cout << i << ". " << std::hex << instructions[i] << '\n';
			}
			std::cout << std::dec;
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
