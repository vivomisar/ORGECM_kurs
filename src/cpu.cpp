#include "cpu.h"
#include "instructions.h"
#include <cstdint>
#include <iomanip>
#include <iostream>
CPU::CPU() : pc(-1), tick(0)
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
	funct3 = (instr & 0x7000) >> 12;
	rs1 = (instr & 0xF8000) >> 15;
	rs2 = (instr & 0x1F00000) >> 20;
	switch (opcode)
	{
	case 0x33: // R-type
	{
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

		return new iR_type(this, rd, rs1, rs2, func);
	}
	case 0x13: // I-type
	{
		// TODO: imm should be sign-extended
		imm = (instr & 0xFFF00000) >> 20;
		uint32_t (*func)(uint32_t, uint16_t) = nullptr;
		switch (funct3)
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
		return new iI_type(this, false, 0, rd, rs1, imm, func);
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

		return new iS_type(this, rs1, rs2, imm, numBytes);
	}
	case 0x63: // B-type (SB-type)
	{
		imm = ((rd & 0x1) << 11) + ((rd & 0x1E) << 1) + ((funct7 & 0x80) << 12) + ((funct7 & 0x3F) << 5);
		bool (*func)(uint32_t, uint32_t) = nullptr;
		switch (funct3)
		{
		case 0x0:
			func = [](uint32_t a, uint32_t b) -> bool { return a == b; };
			break;
		case 0x1:
			func = [](uint32_t a, uint32_t b) -> bool { return a != b; };
			break;
		case 0x4:
			func = [](uint32_t a, uint32_t b) -> bool { return (int32_t)a < (int32_t)b; };
			break;
		case 0x6:
			func = [](uint32_t a, uint32_t b) -> bool { return a < b; };
			break;
		case 0x5:
			func = [](uint32_t a, uint32_t b) -> bool { return (int32_t)a <= (int32_t)b; };
			break;
		case 0x7:
			func = [](uint32_t a, uint32_t b) -> bool { return a <= b; };
			break;
		default:
			throw "Invalid operation";
		}
		return new iB_type(this, rs1, rs2, imm, func);
	}

	case 0x37: // U-type (LUI)
	{
		imm = (instr & 0xFFFFF000) << 12;
		return new iU_type(this, rd, imm);
	}
	case 0x6F: { // J-type (UJ-type)
		imm =
		    (instr & 0xFF000) + ((instr & 0x100000) >> 9) + ((instr & 0x7FE00000) >> 20) + ((instr & 0x80000000) >> 11);
		return new iJ_type(this, rd, imm);
	}
	case 0x3: { // I-type (load)
		uint8_t numBytes = 0;
		imm = (instr & 0xFFF00000) >> 20;
		uint32_t (*func)(uint32_t, uint16_t) = [](uint32_t a, uint16_t b) -> uint32_t { return a + b; };
		switch (funct3)
		{
		case 0x0:
			numBytes = 1;
			break;
		case 0x1:
			numBytes = 2;
			break;
		case 0x2:
			numBytes = 4;
			break;
			break;
		default:
			throw "Invalid operation";
		}
		return new iI_type(this, true, numBytes, rd, rs1, imm, func);
	}
	}
	return nullptr;
}

void CPU::pipeline(const std::vector<uint32_t> &instr, uint16_t ticks)
{
	uint16_t i = 0;
	while (i < ticks)
	{
		std::cout << tick << " tick\n";
		if (toWM)
		{
			toWM->writeMem();
		}
		if (toWR)
		{
			toWR->writeReg();
			toWR->WAR();
		}
		if (toExec)
		{
			toExec->exec();
		}
		if (toLoad)
		{
			toLoad->load();
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
		if (pc == instr.size() || getInWrite(32))
		{
			toLoad = nullptr;
		}
		else
		{
			toLoad = fetch(instr[++pc]);
		}
		++i;
		++tick;
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
		std::cout << std::left << "│ x" << std::setw(7) << std::dec << i << " │ " << std::setw(10) << x[i++] << " │ "
		          << std::left << "x" << std::setw(7) << i << " │ " << std::setw(10) << x[i] << " │\n";
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
		std::cout << std::hex << i + begin << ": " << std::hex << +mem->get(i, false) << "\n";
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

uint8_t CPU::getMem(uint32_t addr) const
{
	try
	{
		return mem->get(addr, true);
	}
	catch (const char *err)
	{
		std::cout << err << '\n';
	}
	return 0;
}

void CPU::setInWrite(uint8_t reg, int8_t state)
{
	if (reg == 0)
	{
		return;
	}
	inWrite[reg] += state;
}

void CPU::setInRead(uint8_t reg, int8_t state)
{
	if (reg == 0)
	{
		return;
	}
	inRead[reg] += state;
}

bool CPU::getInWrite(uint8_t reg)
{
	return inWrite[reg];
}

bool CPU::getInRead(uint8_t reg)
{
	return inRead[reg];
}

void CPU::offsetPC(uint32_t offset)
{
	pc += offset / 4;
}

uint32_t CPU::getPC()
{
	return pc * 4;
}

void CPU::reset()
{
	toLoad = nullptr;
	toExec = nullptr;
	toWR = nullptr;
	toWM = nullptr;
	inRead.fill(0);
	inWrite.fill(0);
	pc = -1;
	mem->clear();
	tick = 0;
	for (int i = 0; i < 32; ++i)
	{
		x[i] = 0;
	}
}

CPU::~CPU()
{
	delete mem;
}