#include "mem.h"

Mem::Mem(uint32_t size)
{
	data = new uint8_t[size];
}

uint8_t Mem::get(uint32_t addr)
{
	if (addr >= size)
	{
		throw "Некорректный адрес памяти";
	}
	return data[addr];
}

void Mem::set(uint32_t addr, uint8_t value)
{
	if (addr >= size)
	{
		throw "Некорректный адрес памяти";
	}
	data[addr] = value;
}

Mem::~Mem()
{
	delete[] data;
}