#include "instructions.h"
#include "cpu.h"
#include <iostream>

iR_type::iR_type(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t (*func)(uint32_t, uint32_t))
    : dsti(rd), src1i(rs1), src2i(rs2), func(func)
{
}

void iR_type::load(CPU &cpu)
{
	src1 = cpu.getReg(src1i);
	src2 = cpu.getReg(src2i);
	cpu.setInUse(dsti, true);
	std::cout << "Loaded x" << (uint16_t)src1i << "; Loaded x" << (uint16_t)src2i << '\n';
}

void iR_type::exec()
{
	dst = func(src1, src2);
	std::cout << "Executed instruction\n";
}

void iR_type::writeReg(CPU &cpu)
{
	cpu.setReg(dsti, dst);
	cpu.setInUse(dsti, false);
	std::cout << "Write to x" << (uint16_t)dsti << '\n';
}

iI_type::iI_type(uint8_t rd, uint8_t rs1, uint16_t imm, uint32_t (*func)(uint32_t, uint16_t))
    : dsti(rd), src1i(rs1), imm(imm), func(func)
{
}

void iI_type::load(CPU &cpu)
{
	src1 = cpu.getReg(src1i);
	cpu.setInUse(dsti, true);
	std::cout << "Loaded x" << (uint16_t)src1i << '\n';
}

void iI_type::exec()
{
	dst = func(src1, imm);
	std::cout << "Executed instruction\n";
}

void iI_type::writeReg(CPU &cpu)
{
	cpu.setReg(dsti, dst);
	cpu.setInUse(dsti, false);
	std::cout << "Write to x" << (uint16_t)dsti << '\n';
}

iS_type::iS_type(uint8_t rs1, uint8_t rs2, uint16_t imm, uint8_t numBytes)
    : imm(imm), src1i(rs1), src2i(rs2), numBytes(numBytes)
{
}

void iS_type::load(CPU &cpu)
{
	offset = cpu.getReg(src1i);
	src = cpu.getReg(src2i);
	cpu.setInUse(src1i, true);
	cpu.setInUse(src2i, true);
	std::cout << "Loaded x" << (uint16_t)src1i << "; Loaded x" << (uint16_t)src2i << '\n';
}

void iS_type::writeReg(CPU &cpu)
{
}

void iS_type::exec()
{
	dsti = imm + offset;
	std::cout << "Executed instruction\n";
}

void iS_type::writeMem(CPU &cpu)
{
	for (int i = 0; i < numBytes; ++i)
	{
		cpu.setMem(dsti, src);
		src = src >> 8;
	}
}