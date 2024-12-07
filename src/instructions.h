#ifndef INSTR_H
#define INSTR_H
#include "cpu.h"
#include <cstdint>

class Instruction
{
  public:
	virtual void load(CPU &) = 0;
	virtual void exec() = 0;
	virtual void writeReg(CPU &) = 0;
	virtual void writeMem(CPU &) = 0;
	virtual ~Instruction()
	{
	}
};

class iR_type : public Instruction
{
  private:
	uint8_t dsti, src1i, src2i;
	uint32_t dst, src1, src2;
	uint32_t (*func)(uint32_t, uint32_t);

  public:
	iR_type(uint8_t, uint8_t, uint8_t, uint32_t (*func)(uint32_t, uint32_t));
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override {};
};

class iI_type : public Instruction
{
  private:
	uint8_t dsti, src1i;
	uint16_t imm;
	uint32_t dst, src1;
	uint32_t (*func)(uint32_t, uint16_t);

  public:
	iI_type(uint8_t, uint8_t, uint16_t, uint32_t (*func)(uint32_t, uint16_t));
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override {};
};
class iS_type : public Instruction
{
  private:
	uint8_t src1i, src2i, numBytes;
	uint16_t imm;
	uint32_t dsti, src, offset;

  public:
	iS_type(uint8_t rs1, uint8_t rs2, uint16_t imm, uint8_t numBytes);
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override;
};
class iU_type : public Instruction
{
  public:
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override;
};
class iB_type : public Instruction
{
  public:
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override;
};
class iJ_type : public Instruction
{
  public:
	void load(CPU &) override;
	void exec() override;
	void writeReg(CPU &) override;
	void writeMem(CPU &) override;
};
#endif