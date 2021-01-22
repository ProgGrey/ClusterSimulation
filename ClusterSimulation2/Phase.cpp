#include "Statistic.hpp"
#include <iostream>

using namespace std;

#define computeElementsNumber(x) (((x) >= (servCount)) ? (elSize) : (Phase::pow(servCount,x)))

Phase& Phase::operator=(const Phase& right)
{
    arrSize = right.arrSize;
    elSize = right.elSize;
    servCount = right.servCount;
    zero = 0;
    p_n_stat = (double**)malloc(sizeof(double*) * arrSize);
    //в цикле копируем все массивы
    for (unsigned int k = 0; k < arrSize; k++) {
        p_n_stat[k] = (double*)malloc(computeElementsNumber(k) * sizeof(double));
        memcpy(p_n_stat[k], right.p_n_stat[k], computeElementsNumber(k) * sizeof(double));
    }
    return *this;
}

Phase& Phase::operator*=(double right)
{
    for (unsigned int k = 0; k < arrSize; k++) {
        for (unsigned int i = 0; i < computeElementsNumber(k); i++) {
            p_n_stat[k][i] *= right;
        }
    }
    return *this;
}

Phase& Phase::operator+=(const Phase& right)
{
    // Возможны 2 варианта:
    if (arrSize >= right.arrSize) {
        // Если размер текущего массива больше или равен размеру прибаляемого,
        // то просто поэлементно складываем с текущим массивом правый массив
        for (unsigned int k = 1; k < right.arrSize; k++) {
            for (unsigned int i = 0; i < computeElementsNumber(k); i++) {
                p_n_stat[k][i] += right.p_n_stat[k][i];
            }
        }
    }
    else {
        // Если размер текущего массива меньше, мы должны выделить ему больше памяти
        // и сложить только те элементы, которые были в левом массиве изначально.
        // Сначала сложим теэлементы, которые уже есть:
        for (unsigned int k = 1; k < arrSize; k++) {
            for (unsigned int i = 0; i < computeElementsNumber(k); i++) {
                p_n_stat[k][i] += right.p_n_stat[k][i];
            }
        }
        // Теперь выделим памяти и скопируем оставшиеся элементы
        double** tmp = (double**)malloc(sizeof(double*) * right.arrSize);
        memcpy(tmp, p_n_stat, arrSize * sizeof(double*));
        for (unsigned int k = arrSize; k < right.arrSize; k++) {
            tmp[k] = (double*)malloc(computeElementsNumber(k) * sizeof(double));
            memcpy(tmp[k], right.p_n_stat[k], computeElementsNumber(k) * sizeof(double));
        }
        free(p_n_stat);
        p_n_stat = tmp;
        arrSize = right.arrSize;
    }
    return *this;
}

Phase::Phase(const Phase& obj)
{
    arrSize = obj.arrSize;
    elSize = obj.elSize;
    servCount = obj.servCount;
    zero = 0;
    p_n_stat = (double**)malloc(sizeof(double*) * arrSize);
    //в цикле копируем все массивы
    for (unsigned int k = 0; k < arrSize; k++) {
        p_n_stat[k] = (double*)malloc(computeElementsNumber(k) * sizeof(double));
        memcpy(p_n_stat[k], obj.p_n_stat[k], computeElementsNumber(k) * sizeof(double));
    }
}

Phase::Phase()
{
    elSize = 0;
    zero = 0;
    servCount = 0;
    arrSize = 0;
    p_n_stat = NULL;
}

Phase::Phase(uint8_t serversCount)
{
    elSize = this->pow(serversCount, serversCount);
    zero = 0;
    servCount = serversCount;
    arrSize = 0;
    p_n_stat = NULL;
}

void Phase::deleteArr()
{
    for (unsigned int k = 1; k < arrSize; k++) {
        free(p_n_stat[k]);
    }
    free(p_n_stat);
}

Phase::~Phase()
{
    deleteArr();
}

unsigned long int Phase::pow(unsigned int a, uint8_t p)
{
    unsigned long int res = a;
    while (p > 1)
    {
        if (p & 1) {
            // p не делится на 2
            p--;
            res *= a;
        }
        else {
            // p делится на 2
            p >>= 1;
            res *= a;
            a *= a;
        }
    }
    return res;
}
double& Phase::operator()(unsigned int x, unsigned int y)
{
    unsigned int maxY;
    if (x < servCount) {
        maxY = pow(servCount, (uint8_t)x);
    }
    else {
        maxY = elSize;
    }
    if (y >= maxY) {
        zero = 0;
        return zero;
    }
    if (x >= arrSize) {
        double** tmp = (double**)malloc(sizeof(double*) * ((size_t)x + 1));
        memcpy(tmp, p_n_stat, arrSize * sizeof(double*));
        for (unsigned int k = arrSize; k <= x; k++) {
            tmp[k] = (double*)calloc(computeElementsNumber(k), sizeof(double));
        }
        free(p_n_stat);
        p_n_stat = tmp;
        arrSize = x + 1;
    }
    return p_n_stat[x][y];
}

void Phase::print()
{
    for (unsigned int i = 0; i < elSize; i++) {
        for (unsigned int k = 0; k < arrSize; k++) {
            cout << (*this)(k, i) << "\t";
        }
        cout << "|" << endl;
    }
}

void Phase::addTime(uint8_t* arr, unsigned int x, double time)
{
    // Вычислим позицию состояния в столбце
    int64_t bound = servCount > x ? x : servCount;
    unsigned int y = 0;
    for (int_fast64_t k = 0; k < (bound - 1); k++) {
        y += (arr[k] - 1);
        y *= servCount;
    }
    y += arr[bound - 1] - 1;
    // Добавим время в нужное место
    (*this)(x, y) += time;
}

unsigned int Phase::getColumnSize()const
{
    return elSize;
}

unsigned int Phase::size() const
{
    return arrSize;
}