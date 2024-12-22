#include "instructions.h"
#include "cpu.h"
#include <cstdint>
#include <iostream>

iR_type::iR_type(CPU *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t (*func)(uint32_t, uint32_t))
    : dsti(rd), src1i(rs1), src2i(rs2), func(func)
{
	this->cpu = cpu;
	cpu->setInRead(src1i, true);
	cpu->setInRead(src2i, true);
	cpu->setInWrite(dsti, true);
}

void iR_type::load()
{
	src1 = cpu->getReg(src1i);
	src2 = cpu->getReg(src2i);
	cpu->setInRead(src1i, false);
	cpu->setInRead(src2i, false);
	std::cout << "Loaded x" << (uint16_t)src1i << "; Loaded x" << (uint16_t)src2i << '\n';
}

void iR_type::exec()
{
	dst = func(src1, src2);
	std::cout << "Executed instruction\n";
}

void iR_type::writeReg()
{
	cpu->setReg(dsti, dst);
	cpu->setInWrite(dsti, false);
	std::cout << "Write to x" << (uint16_t)dsti << '\n';
}

void iR_type::writeMem()
{
}

void iR_type::WAR()
{
	if (cpu->getInRead(dsti))
	{
		std::cerr << "DATA HAZARD (Write After Read) on register x" << +dsti << "\n";
	}
}

void iR_type::WAW()
{
	if (cpu->getInWrite(dsti))
	{
		std::cerr << "DATA HAZARD (Write After Write) on register x" << +dsti << "\n";
	}
}

iR_type::~iR_type()
{
}

iI_type::iI_type(CPU *cpu, uint8_t rd, uint8_t rs1, uint16_t imm, uint32_t (*func)(uint32_t, uint16_t))
    : dsti(rd), src1i(rs1), imm(imm), func(func)
{
	this->cpu = cpu;
	cpu->setInRead(src1i, true);
	cpu->setInWrite(dsti, true);
}

void iI_type::load()
{
	src1 = cpu->getReg(src1i);
	cpu->setInRead(src1i, false);
	std::cout << "Loaded x" << (uint16_t)src1i << '\n';
}

void iI_type::exec()
{
	dst = func(src1, imm);
	std::cout << "Executed instruction\n";
}

void iI_type::writeReg()
{
	cpu->setReg(dsti, dst);
	cpu->setInWrite(dsti, false);
	std::cout << "Write to x" << (uint16_t)dsti << '\n';
}

void iI_type::writeMem()
{
}

void iI_type::WAR()
{
	if (cpu->getInRead(dsti))
	{
		std::cerr << "DATA HAZARD (Write After Read) on register x" << +dsti << "\n";
	}
}

void iI_type::WAW()
{
	if (cpu->getInWrite(dsti))
	{
		std::cerr << "DATA HAZARD (Write After Write) on register x" << +dsti << "\n";
	}
}

iI_type::~iI_type()
{
}

iS_type::iS_type(CPU *cpu, uint8_t rs1, uint8_t rs2, uint16_t imm, uint8_t numBytes)
    : imm(imm), src1i(rs1), src2i(rs2), numBytes(numBytes)
{
	this->cpu = cpu;
	cpu->setInRead(src1i, true);
	cpu->setInRead(src2i, true);
}

void iS_type::load()
{
	offset = cpu->getReg(src1i);
	src = cpu->getReg(src2i);
	cpu->setInRead(src1i, false);
	cpu->setInRead(src2i, false);
	std::cout << "Loaded x" << (uint16_t)src1i << "; Loaded x" << (uint16_t)src2i << '\n';
}

void iS_type::writeReg()
{
}

void iS_type::WAR()
{
}

void iS_type::WAW()
{
}

void iS_type::exec()
{
	dsti = imm + offset;
	std::cout << "Executed instruction\n";
}

void iS_type::writeMem()
{
	for (int i = 0; i < numBytes; ++i)
	{
		cpu->setMem(dsti, src);
		src = src >> 8;
	}
}

iS_type::~iS_type()
{
}

iB_type::iB_type(CPU *cpu, uint8_t rs1, uint8_t rs2, uint16_t imm, bool (*func)(uint32_t, uint32_t))
    : src1i(rs1), src2i(rs2), imm(), func(func)
{
	this->cpu = cpu;
	cpu->setInRead(src1i, true);
	cpu->setInRead(src2i, true);
	cpu->setInWrite(32, true);
}

void iB_type::load()
{
	src1 = cpu->getReg(src1i);
	src2 = cpu->getReg(src2i);
	if (cpu->getInWrite(src1i))
	{
		std::cerr << "CONTROL HAZARD on register x" << +src1i << "\n";
	}
	if (cpu->getInWrite(src2i))
	{
		std::cerr << "CONTROL HAZARD on register x" << +src2i << "\n";
	}
	cpu->setInRead(src1i, false);
	cpu->setInRead(src2i, false);
	std::cout << "Loaded x" << (uint16_t)src1i << "; Loaded x" << (uint16_t)src2i << '\n';
}

void iB_type::exec()
{
	if (func(src1, src2))
	{
		offset = imm;
	}
	else
	{
		offset = 0;
	}
	std::cout << "Executed instruction\n";
}

void iB_type::writeReg()
{
	cpu->offsetPC(offset);
	cpu->setInWrite(32, false);
	std::cout << "Write to pc\n";
}

void iB_type::writeMem()
{
}

void iB_type::WAR()
{
}

void iB_type::WAW()
{
}

iB_type::~iB_type()
{
}

iU_type::iU_type(CPU *cpu, uint8_t dsti, uint32_t imm) : dsti(dsti), imm(imm)
{
	this->cpu = cpu;
	cpu->setInWrite(dsti, true);
}

void iU_type::load()
{
}

void iU_type::exec()
{
}

void iU_type::writeMem()
{
}
void iU_type::writeReg()
{
	cpu->setReg(dsti, imm);
	cpu->setInWrite(dsti, false);
}