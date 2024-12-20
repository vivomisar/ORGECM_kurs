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
	uint16_t tick;
	std::bitset<32> inRead;
	std::bitset<32> inWrite;
	Mem *mem;
	Instruction *toLoad = nullptr;
	Instruction *toExec = nullptr;
	Instruction *toWR = nullptr;
	Instruction *toWM = nullptr;
	Instruction *fetch(uint32_t);

  public:
	CPU();
	void reset();
	void pipeline(const std::vector<uint32_t> &, uint16_t ticks = 1);
	uint32_t getReg(uint8_t) const;
	uint8_t getMem(uint32_t) const;
	void setReg(uint8_t, uint32_t);
	void setMem(uint32_t, uint8_t);
	void printMem(uint32_t, uint32_t);
	void printRegs();
	void setInRead(uint8_t, bool);
	void setInWrite(uint8_t, bool);
	bool getInRead(uint8_t);
	bool getInWrite(uint8_t);
	~CPU();
};
#endif