#include "ExponentialDistribution.hpp"
#include <float.h>

ExponentialDistribution::ExponentialDistribution()
{
	std::random_device rd;
	uint64_t seed = rd();
	gen1.seed(seed);
	gen2.seed(seed);
	max = (double)gen1.max();
	lambda = 1;
	this->generator = &ExponentialDistribution::gen;
}

ExponentialDistribution::ExponentialDistribution(double lambda)
{
	std::random_device rd;
	uint64_t seed = rd();
	gen1.seed(seed);
	gen2.seed(seed);
	max = (double)gen1.max();
	this->lambda = lambda;
	this->generator = &ExponentialDistribution::gen;
}

double ExponentialDistribution::operator()()
{
	return (this->*generator)();
}

// Заменяет ноль на нименьшее число больше нуля.
// Нужно при генерации случайных чисел, чтобы 
// генератор Exp не выдал +inf
inline double removeZero(double a) 
{
	double eps = DBL_TRUE_MIN;
	return a + eps;
}

double ExponentialDistribution::altGen()
{
	// Нормализуем генерируемые значения
	double tmp = (double)gen2();
	tmp /= max;
	// Вычислим по методу обратной функции
	return -log(removeZero(1 - tmp)) / lambda;
}

double ExponentialDistribution::gen()
{
	// Нормализуем генерируемые значения
	double tmp = (double)gen1();
	tmp /= max;
	// Вычислим по методу обратной функции
	return -log(removeZero(tmp)) / lambda;
}

void ExponentialDistribution::useAlternativeGenerator()
{
	this->generator = &ExponentialDistribution::altGen;
}
void ExponentialDistribution::useStandartGenerator()
{
	this->generator = &ExponentialDistribution::gen;
}

void ExponentialDistribution::syncGenerators()
{
	gen1 = gen2;
}