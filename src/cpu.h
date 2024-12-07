#ifndef CPU_H
#define CPU_H

#include "mem.h"
#include <bitset>
#include <cstdint>
#include <vector>

class Instruction;

class CPU
{
  private:
	uint32_t x[32] = {};
	uint32_t pc;
	std::bitset<32> inUse;
	Mem *mem;
	Instruction *fetch(uint32_t);

  public:
	CPU();
	void pipeline(const std::vector<uint32_t> &, uint16_t ticks = 5);
	uint32_t getReg(uint8_t) const;
	void setReg(uint8_t, uint32_t);
	void setMem(uint32_t);
	void printRegs();
	void setInUse(uint8_t, bool);
	~CPU();
};
#endif