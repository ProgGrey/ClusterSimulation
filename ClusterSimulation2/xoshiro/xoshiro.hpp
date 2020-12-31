#pragma once
#include <cstdint>

//  ���������� xorshiro256+
// ��. http://prng.di.unimi.it/
class  xoshiro256plus
{
public:
	// �������������������
	static uint64_t min();
	// ������������ ��������
	static uint64_t max();
	// ��������� ����� ����������
	void seed(uint64_t s);
	// ��������� �����
	uint64_t operator()();
	// ����������� �� ���������
	xoshiro256plus();
	// �����������, ��������������� �����
	explicit xoshiro256plus(uint64_t s);
private:
	uint64_t data[4];
};
