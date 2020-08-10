#pragma once

#include "xoshiro.hpp"
#include <random>

class  ProbabilityTest
{
public:
	 ProbabilityTest();
	 // ���������� true � ������������ p
	 // � false � ������������ 1-p
	 bool operator()(double p);
private:
	xoshiro256plus gen;
	double max;
};