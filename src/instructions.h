#ifndef INSTR_H
#define INSTR_H
#include "cpu.h"
#include <cstdint>

class Instruction
{
  protected:
	CPU *cpu = nullptr;

  public:
	virtual void load() = 0;
	virtual void exec() = 0;
	virtual void writeReg() = 0;
	virtual void writeMem() = 0;
	virtual void WAR() = 0;
	virtual void WAW() = 0;
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
	iR_type(CPU *, uint8_t, uint8_t, uint8_t, uint32_t (*func)(uint32_t, uint32_t));
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iR_type();
};

class iI_type : public Instruction
{
  private:
	uint8_t dsti, src1i;
	uint16_t imm;
	uint32_t dst, src1;
	uint32_t (*func)(uint32_t, uint16_t);

  public:
	iI_type(CPU *, uint8_t, uint8_t, uint16_t, uint32_t (*func)(uint32_t, uint16_t));
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iI_type();
};
class iS_type : public Instruction
{
  private:
	uint8_t src1i, src2i, numBytes;
	uint16_t imm;
	uint32_t dsti, src, offset;

  public:
	iS_type(CPU *, uint8_t rs1, uint8_t rs2, uint16_t imm, uint8_t numBytes);
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iS_type();
};
class iU_type : public Instruction
{
	uint8_t dsti;
	uint32_t imm;

  public:
	iU_type(CPU *, uint8_t rd, uint32_t imm);
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iU_type();
};
class iB_type : public Instruction
{
  private:
	uint32_t src1, src2;
	uint16_t imm, offset;
	uint8_t src1i, src2i;
	bool (*func)(uint32_t, uint32_t);

  public:
	iB_type(CPU *, uint8_t rs1, uint8_t rs2, uint16_t imm, bool (*func)(uint32_t, uint32_t));
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iB_type();
};
class iJ_type : public Instruction
{
  public:
	void load() override;
	void exec() override;
	void writeReg() override;
	void writeMem() override;
	void WAR() override;
	void WAW() override;
	~iJ_type();
};
#endif