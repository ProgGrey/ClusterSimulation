#pragma once
#include "xoshiro.hpp"
#include <random>

class ExponentialDistribution
{
public:
	ExponentialDistribution();
	ExponentialDistribution(double lambda);
	
	double lambda;
	double operator()();

	void useAlternativeGenerator();
	void useStandartGenerator();
private:
	xoshiro256plus gen1;
	xoshiro256plus gen2;
	double max;
	double altGen();
	double gen();

	typedef double(ExponentialDistribution::* ExponencialGeneratorPointer)();
	ExponencialGeneratorPointer generator;
};

