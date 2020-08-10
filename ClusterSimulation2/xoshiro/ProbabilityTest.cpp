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
	// Ќормализуем генерируемые значени€
	double tmp = gen();
	tmp /= max;
	// √енерируем true с веро€тностью p
	// и false с веро€тностью 1-p
	if (tmp <= p) {
		return true;
	} else {
		return false;
	}
}