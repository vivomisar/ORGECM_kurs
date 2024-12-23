#ifndef CPU_H
#define CPU_H

#include "mem.h"
#include <array>
#include <cstdint>
#include <vector>

class Instruction;

class CPU
{
  private:
	uint32_t x[32] = {};
	uint32_t pc;
	uint16_t tick;
	std::array<uint8_t, 33> inRead;
	std::array<uint8_t, 33> inWrite;
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
	void offsetPC(uint32_t);
	uint32_t getPC();
	void setMem(uint32_t, uint8_t);
	void printMem(uint32_t, uint32_t);
	void printRegs();
	void setInRead(uint8_t, int8_t);
	void setInWrite(uint8_t, int8_t);
	bool getInRead(uint8_t);
	bool getInWrite(uint8_t);
	~CPU();
};
#endif