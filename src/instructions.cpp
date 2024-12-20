#include "instructions.h"
#include "cpu.h"
#include <iostream>

iR_type::iR_type(CPU *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t (*func)(uint32_t, uint32_t))
    : dsti(rd), src1i(rs1), src2i(rs2), func(func)
{
	this->cpu = cpu;
	cpu->setInRead(src1i, true);
	cpu->setInRead(src2i, true);
}

void iR_type::load()
{
	src1 = cpu->getReg(src1i);
	src2 = cpu->getReg(src2i);
	cpu->setInRead(src1i, false);
	cpu->setInRead(src2i, false);
	cpu->setInWrite(dsti, true);
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

void iR_type::RAW()
{
	if (cpu->getInWrite(src1i))
	{
		std::cerr << "DATA HAZARD (Read After Write) on register x" << +src1i << "\n";
	}
	if (cpu->getInWrite(src2i))
	{
		std::cerr << "DATA HAZARD (Read After Write) on register x" << +src2i << "\n";
	}
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

void iI_type::RAW()
{
	if (cpu->getInWrite(src1i))
	{
		std::cerr << "DATA HAZARD (Read After Write) on register x" << +src1i << "\n";
	}
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

void iS_type::RAW()
{
	if (cpu->getInWrite(src1i))
	{
		std::cerr << "DATA HAZARD (Read After Write) on register x" << +src1i << "\n";
	}
	if (cpu->getInWrite(src2i))
	{
		std::cerr << "DATA HAZARD (Read After Write) on register x" << +src2i << "\n";
	}
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