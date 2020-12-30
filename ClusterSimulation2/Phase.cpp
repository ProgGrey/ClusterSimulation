#include "Statistic.hpp"
#include <iostream>

using namespace std;

Phase::Phase()
{
    elSize = 0;
    elCount = 0;
    zero = 0;
    servCount = 0;
}

Phase::Phase(uint8_t serversCount)
{
    elSize = this->pow(serversCount, serversCount);
    elCount = 0;
    zero = 0;
    servCount = serversCount;
}

void Phase::doppelgang(const Phase& obj)
{
    zero = 0;
    servCount = obj.servCount;
    elSize = obj.elSize;
    elCount = obj.elCount;
    p_mu_stat = obj.p_mu_stat;

    if (elSize != 0) {
        // Скопируем массив
        unsigned int k = 1;
        p_n_stat.reserve(elCount);
        for (; k < servCount; k++)
        {
            unsigned int memSize = sizeof(double) * pow(k, (uint8_t)k);
            double* tmp = (double*)malloc(memSize);
            memcpy(tmp, obj.p_n_stat[k], memSize);
            p_n_stat[k] = tmp;
        }
        for (; k < elCount; k++)
        {
            unsigned int memSize = sizeof(double) * elSize;
            double* tmp = (double*)malloc(memSize);
            memcpy(tmp, obj.p_n_stat[k], memSize);
            p_n_stat[k] = tmp;
        }
    }
}

Phase::Phase(const Phase& obj)
{
    doppelgang(obj);
}

Phase& Phase::operator=(const Phase& right)
{
    this->~Phase();
    doppelgang(right);
    return(*this);
}

Phase::~Phase()
{
    for (unsigned int k = 1; k < elCount; k++) {
        free(p_n_stat[k]);
    }
    p_n_stat.clear();
}

inline unsigned int Phase::pow(unsigned int a, uint8_t p)
{
    unsigned int res = a;
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
inline double& Phase::operator()(unsigned int x, unsigned int y)
{
    unsigned int maxY;
    if (x < servCount) {
        maxY = pow(x, (uint8_t)x);
    }
    else {
        maxY = elSize;
    }
    if (y >= maxY) {
        zero = 0;
        return zero;
    }

    if (x >= elCount) {
        while (elCount <= x)
        {
            double* tmp = (double*)calloc(maxY, sizeof(double));
            p_n_stat.push_back(tmp);
            elCount++;
        }
    }
    return p_n_stat[x][y];
}

void Phase::print()
{
    for (unsigned int i = 0; i < elSize; i++) {
        for (unsigned int k = 0; k < elCount; k++) {
            cout << (*this)(k, i) << "\t";
        }
        cout << endl;
    }
}

void Phase::addTime(uint8_t* arr, unsigned int x, double time)
{
    // Вычислим позицию состояния в столбце
    uint64_t bound = servCount > x ? x : servCount;
    unsigned int y = 0;
    for (uint_fast64_t k = 0; k < (bound - 1); k++) {
        y += (arr[k] - 1);
        y *= servCount;
    }
    y += arr[bound - 1] - 1;
    // Добавим время в нужное место
    (*this)(x, y) += time;
}