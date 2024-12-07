#include "mem.h"

Mem::Mem(uint8_t size)
{
	data = new uint8_t[size];
}

Mem::~Mem()
{
	delete[] data;
}