#pragma once
#include <cstring>
#include <new>

// ������������ ������ ��� ������� �����!!!
// �.�. ��� ����������� � �����������.
// �� ��������� ��� �������� ��������
template<typename T, unsigned int maxSize>
class DynamicArraySimple
{
private:
	// ����� ������
	unsigned int len;
	// �����
	T* buffer;
public:
	
	DynamicArraySimple()
	{
		buffer = new T[1];
		memset(buffer, 0, sizeof(T));
		len = 1;
	}

	~DynamicArraySimple()
	{
		delete[] buffer;
		buffer = NULL;
	}

	T& operator [](const unsigned int index)
	{
		if (index >= len) {
			if (index < maxSize) {
				// �������� ����� �����
				unsigned int newLen = index << 1;
				// ����� �� ������ ��������� ������������ ������ �������
				newLen = ((newLen > maxSize) ? maxSize : newLen);
				// �������� ����� ����� � �������� � ���� ������
				T* tmp = new T[newLen];
				memset(tmp, 0, newLen*sizeof(T));
				memcpy(tmp, buffer, len * sizeof(T));
				delete[] buffer;
				buffer = tmp;
				len = newLen;
				return buffer[index];
			} else {
				throw std::bad_array_new_length();
			}
		}
		else {
			return buffer[index];
		}
	}
	// ����������� �����������
	DynamicArraySimple<T, maxSize>(const DynamicArraySimple<T, maxSize>& initialize)
	{
		// ������ ����� �������
		len = initialize.len;
		delete[] buffer;
		buffer = new T[len];
		memset(buffer, 0, len * sizeof(T));
		memcpy(buffer, initialize.buffer, len * sizeof(T));
	}

	// �������� ������������
	DynamicArraySimple<T, maxSize>& operator=(const DynamicArraySimple<T, maxSize>& right) {
		//�������� �� ����������������
		if (this == &right) {
			return *this;
		}
		// ������ ����� �������
		len = right.len;
		delete[] buffer;
		buffer = new T[len];
		memset(buffer, 0, len * sizeof(T));
		memcpy(buffer, right.buffer, len * sizeof(T));
		return *this;
	}

	// ������ �������
	unsigned int size() {
		return len;
	}
};