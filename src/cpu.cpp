#include "cpu.h"
#include "instructions.h"
#include <cstdint>
#include <iomanip>
#include <iostream>
CPU::CPU() : pc(0)
{
	mem = new Mem(32);
}

Instruction *CPU::fetch(uint32_t instr)
{
	uint8_t opcode, rd, rs1, rs2, funct3, funct7;
	uint32_t imm;
	opcode = instr & 0x7F;
	rd = (instr & 0xF80) >> 7;
	funct7 = (instr & 0xFE000000) >> 25;
	rs1 = (instr & 0xF8000) >> 15;
	rs2 = (instr & 0x1F00000) >> 20;
	switch (opcode)
	{
	case 0x33: // R-type
	{

		funct3 = (instr & 0x7000) >> 12;
		uint32_t (*func)(uint32_t, uint32_t) = nullptr;
		switch (funct3 + (funct7 << 12))
		{
		case 0x0:
			func = [](uint32_t a, uint32_t b) -> uint32_t {
				//
				return a + b;
			};
			break;
		case 0x7:
			func = [](uint32_t a, uint32_t b) -> uint32_t { return a & b; };
			break;
		case 0x6:
			func = [](uint32_t a, uint32_t b) -> uint32_t { return a | b; };
			break;
		case 0x5:
			func = [](uint32_t a, uint32_t b) -> uint32_t {
				uint8_t uimm5 = b & 0x1F;
				return a >> uimm5;
			};
			break;
		case 0x4:
			func = [](uint32_t a, uint32_t b) -> uint32_t { return a ^ b; };
			break;
		case 0x1:
			func = [](uint32_t a, uint32_t b) -> uint32_t {
				uint8_t uimm5 = b & 0x1F;
				return a << uimm5;
			};
			break;
		default:
			throw "Invalid operation";
		}

		return new iR_type(rd, rs1, rs2, func);
	}
	case 0x13: // I-type
	{
		// TODO: imm should be sign-extended
		imm = (instr & 0xFFF00000) >> 20;
		uint32_t (*func)(uint32_t, uint16_t) = nullptr;
		switch (funct3 + (funct7 << 12))
		{
		case 0x0:
			func = [](uint32_t a, uint16_t b) -> uint32_t {
				//
				return a + b;
			};
			break;
		case 0x7:
			func = [](uint32_t a, uint16_t b) -> uint32_t { return a & b; };
			break;
		case 0x6:
			func = [](uint32_t a, uint16_t b) -> uint32_t { return a | b; };
			break;
		case 0x5:
			func = [](uint32_t a, uint16_t b) -> uint32_t {
				uint8_t uimm5 = b & 0x1F;
				return a >> uimm5;
			};
			break;
		case 0x4:
			func = [](uint32_t a, uint16_t b) -> uint32_t { return a ^ b; };
			break;
		case 0x1:
			func = [](uint32_t a, uint16_t b) -> uint32_t {
				uint8_t uimm5 = b & 0x1F;
				return a << uimm5;
			};
			break;
		default:
			throw "Invalid operation";
		}
		return new iI_type(rd, rs1, imm, func);
	}
	case 0x23: // S-type
	{
		imm = funct7 + rd;
		uint8_t numBytes;
		switch (funct3)
		{
		case 0x2:
			numBytes = 4;
			break;
		case 0x1:
			numBytes = 2;
			break;
		case 0x0:
			numBytes = 1;
			break;
		default:
			throw "Invalid operation";
		}

		return new iS_type(rs1, rs2, imm, numBytes);
	}
		/*
	case 0x37: // U-type
		return new iU_type();
	case 0x63: // B-type (SB-type)
		return new iB_type();
	case 0x6F: // J-type (UJ-type)
		return new iJ_type(); */
	}
	return nullptr;
}

void CPU::pipeline(const std::vector<uint32_t> &instr, uint16_t ticks)
{
	uint16_t i = 0;
	static Instruction *toLoad = nullptr;
	static Instruction *toExec = nullptr;
	static Instruction *toWR = nullptr;
	static Instruction *toWM = nullptr;
	while (i <= ticks)
	{
		std::cout << i << " tick\n";
		if (toWM)
		{
			toWM->writeMem(*this);
		}
		if (toWR)
		{
			toWR->writeReg(*this);
		}
		if (toExec)
		{
			toExec->exec();
		}
		if (toLoad)
		{
			toLoad->load(*this);
		}

		/*Next stage*/
		if (toWM)
		{
			delete toWM;
		}
		toWM = toWR;
		toWR = toExec;
		toExec = toLoad;
		/*Fetch*/
		if (pc == instr.size())
		{
			toLoad = nullptr;
		}
		else
		{
			toLoad = fetch(instr[pc++]);
		}
		++i;
	}
}

uint32_t CPU::getReg(uint8_t index) const
{
	if (index < 0 || index > 31)
	{
		throw "Invalid register";
	}
	if (index == 0)
	{
		return 0;
	}
	if (inUse[index])
	{
		std::cerr << "DATA HAZARD\n";
	}
	return x[index];
}

void CPU::setReg(uint8_t index, uint32_t value)
{
	if (index < 0 || index > 31)
	{
		throw "Invalid register";
	}
	if (index == 0)
	{
		return;
	}
	x[index] = value;
}

void CPU::printRegs()
{
	std::cout << "┌──────────┬────────────┬──────────┬────────────┐\n"
	             "│ register │    value   │ register │    value   │\n"
	             "├──────────┼────────────┤──────────┼────────────┤\n";
	for (int i = 0; i < 32; i++)
	{
		std::cout << std::left << "│ x" << std::setw(7) << i << " │ " << std::setw(10) << x[i++] << " │ " << std::left
		          << "x" << std::setw(7) << i << " │ " << std::setw(10) << x[i] << " │\n";
		if (i == 31)
			std::cout << "└──────────┴────────────┴──────────┴────────────┘\n";

		else
			std::cout << "├──────────┼────────────┼──────────┼────────────┤\n";
	}
}

void CPU::printMem(uint32_t begin, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		std::cout << std::hex << i + begin << ": " << std::hex << mem->get(i) << "\n";
	}
}

void CPU::setMem(uint32_t addr, uint8_t value)
{
	try
	{
		mem->set(addr, value);
	}
	catch (const char *err)
	{
		std::cout << err << '\n';
	}
}

void CPU::setInUse(uint8_t reg, bool state)
{
	inUse[reg] = state;
}

CPU::~CPU()
{
	delete mem;
}