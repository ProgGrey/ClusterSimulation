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

// �������� ���� �� ��������� ����� ������ ����.
// ����� ��� ��������� ��������� �����, ����� 
// ��������� Exp �� ����� +inf
inline double removeZero(double a) 
{
	// ��� ������������ �������� ���� (����������������� �����)
	// NB: ��������������� �� �������.
	uint64_t code = 0x1;
	double* p = (double*)&code;
	double eps = *p;
	return a + eps;
}

double ExponentialDistribution::altGen()
{
	// ����������� ������������ ��������
	double tmp = gen2();
	tmp /= max;
	// �������� �� ������ �������� �������
	return -log(removeZero(1 - tmp)) / lambda;
}

double ExponentialDistribution::gen()
{
	// ����������� ������������ ��������
	double tmp = gen1();
	tmp /= max;
	// �������� �� ������ �������� �������
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