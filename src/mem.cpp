#include "mem.h"

Mem::Mem(uint32_t size) : size(size)
{
	data = new uint8_t[size];
	clear();
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

void Mem::clear()
{
	for (uint32_t i = 0; i < size; ++i)
	{
		data[i] = 0;
	}
}

Mem::~Mem()
{
	delete[] data;
}