#pragma once

#include "xoshiro.hpp"
#include <random>

class  ProbabilityTest
{
public:
	 ProbabilityTest();
	 // Генерируем true с вероятностью p
	 // и false с вероятностью 1-p
	 bool operator()(double p);
private:
	xoshiro256plus gen;
	double max;
};