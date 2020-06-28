#include "ExponentialDistribution.hpp"

ExponentialDistribution::ExponentialDistribution()
{
	std::random_device rd;
	uint64_t seed = rd();
	gen1.seed(seed);
	gen2.seed(seed);
	max = gen1.max();
	lambda = 1;
	this->generator = &ExponentialDistribution::gen;
}

ExponentialDistribution::ExponentialDistribution(double l)
{
	std::random_device rd;
	uint64_t seed = rd();
	gen1.seed(seed);
	gen2.seed(seed);
	max = gen1.max();
	lambda = l;
	this->generator = &ExponentialDistribution::gen;
}

double ExponentialDistribution::operator()()
{
	return (this->*generator)();
}
double ExponentialDistribution::altGen()
{
	// ����������� ������������ ��������
	double tmp = gen2();
	tmp /= max;
	// �������� �� ������ �������� �������
	return -log(1 - tmp) / lambda;
}

double ExponentialDistribution::gen()
{
	// ����������� ������������ ��������
	double tmp = gen1();
	tmp /= max;
	// �������� �� ������ �������� �������
	return -log(tmp) / lambda;
}

void ExponentialDistribution::useAlternativeGenerator()
{
	this->generator = &ExponentialDistribution::altGen;
}
void ExponentialDistribution::useStandartGenerator()
{
	this->generator = &ExponentialDistribution::gen;
}