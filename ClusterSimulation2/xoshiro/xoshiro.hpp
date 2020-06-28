#pragma once
#include <cstdint>

//  ���������� xorshiro256+
// ��. http://prng.di.unimi.it/
class  xoshiro256plus
{
public:
	// �������������������
	uint64_t min();
	// ������������ ��������
	uint64_t max();
	// ��������� ����� ����������
	void seed(uint64_t s);
	// ��������� �����
	uint64_t operator()();
	// ����������� �� ���������
	xoshiro256plus();
	// �����������, ��������������� �����
	xoshiro256plus(uint64_t s);
private:
	uint64_t data[4];
};
