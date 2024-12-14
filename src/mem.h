#ifndef MEM_H
#define MEM_H
#include <cstdint>
class Mem
{
  private:
	uint8_t *data;
	uint32_t size;

  public:
	Mem() = delete;
	Mem(uint32_t size);

	uint8_t get(uint32_t addr);
	void set(uint32_t addr, uint8_t data);

	~Mem();
};
#endif