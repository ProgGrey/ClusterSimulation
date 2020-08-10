#include "ProbabilityTest.hpp"

ProbabilityTest::ProbabilityTest()
{
	std::random_device rd;
	uint64_t seed = rd();
	gen.seed(seed);
	max = gen.max();
}

bool ProbabilityTest::operator()(double p)
{
	// ����������� ������������ ��������
	double tmp = gen();
	tmp /= max;
	// ���������� true � ������������ p
	// � false � ������������ 1-p
	if (tmp <= p) {
		return true;
	} else {
		return false;
	}
}