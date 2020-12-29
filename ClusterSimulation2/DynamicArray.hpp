#pragma once
#include <cstring>
#include <new>

// Использовать только для простых типов!!!
// Т.е. без конструтора и деструктора.
// По умолчанию все элементы обнулены
template<typename T, unsigned int maxSize>
class DynamicArraySimple
{
private:
	// Длина буфера
	unsigned int len;
	// Буфер
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
				// Вычислим новую длину
				unsigned int newLen = index << 1;
				// Длина не должна превышать максимальный размер массива
				newLen = ((newLen > maxSize) ? maxSize : newLen);
				// Созданим новый буфер и перенесём в него массив
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
	// Конструктор копирования
	DynamicArraySimple<T, maxSize>(const DynamicArraySimple<T, maxSize>& initialize)
	{
		// Создаём копию объекта
		len = initialize.len;
		delete[] buffer;
		buffer = new T[len];
		memset(buffer, 0, len * sizeof(T));
		memcpy(buffer, initialize.buffer, len * sizeof(T));
	}

	// Оператор присваивания
	DynamicArraySimple<T, maxSize>& operator=(const DynamicArraySimple<T, maxSize>& right) {
		//проверка на самоприсваивание
		if (this == &right) {
			return *this;
		}
		// Создаём копию объекта
		len = right.len;
		delete[] buffer;
		buffer = new T[len];
		memset(buffer, 0, len * sizeof(T));
		memcpy(buffer, right.buffer, len * sizeof(T));
		return *this;
	}

	// Размер массива
	unsigned int size() {
		return len;
	}
};