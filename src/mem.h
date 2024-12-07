#ifndef MEM_H
#define MEM_H
#include <cstdint>
class Mem
{
  private:
	uint8_t *data;

  public:
	Mem() = delete;
	Mem(uint8_t size);

	uint8_t get(uint32_t addr);
	void set(uint32_t addr, uint8_t data);

	~Mem();
};
#endif