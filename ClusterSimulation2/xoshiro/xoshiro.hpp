#pragma once
#include <cstdint>

//  Реализация xorshiro256+
// См. http://prng.di.unimi.it/
class  xoshiro256plus
{
public:
	// Минимальноезначение
	uint64_t min();
	// Максимальное значение
	uint64_t max();
	// Установка зерна генератора
	void seed(uint64_t s);
	// Генерация числа
	uint64_t operator()();
	// конструктор по умолчанию
	xoshiro256plus();
	// Конструктор, устанавливающий зерно
	xoshiro256plus(uint64_t s);
private:
	uint64_t data[4];
};
