#include "xoshiro.hpp"

uint64_t xoshiro256plus::max()
{
	// 2^53-1
	return 0x1FFFFFFFFFFFFF;
}

uint64_t xoshiro256plus::min()
{
	return 0;
}

inline uint64_t rotl(uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

// Генератор чисел
uint64_t xoshiro256plus::operator()()
{
	uint64_t result = data[0] + data[3];
	uint64_t tmp = data[1] << 17;

	data[2] ^= data[0];
	data[3] ^= data[1];
	data[1] ^= data[2];
	data[0] ^= data[3];

	data[2] ^= tmp;
	
	data[3] = rotl(data[3], 45);
	return result >> 11;
}
 
// Для инициализации используется алгоритм SplitMix64
void xoshiro256plus::seed(uint64_t s) {
	uint64_t z;
	for (uint_fast8_t k = 0; k < 4; k++) {
		s += 0x9e3779b97f4a7c15;
		z = s;
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		data[k] = z ^ (z >> 31);
	}
}

xoshiro256plus::xoshiro256plus()
{
	seed(12);
}

xoshiro256plus::xoshiro256plus(uint64_t s)
{
	seed(s);
}