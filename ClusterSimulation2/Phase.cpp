#include "Statistic.hpp"
Phase::Phase()
{
    p_n_stat = NULL;
    serversCount = 0;
}

Phase::Phase(uint64_t servCount)
{
    serversCount = servCount;
    p_n_stat = new int[servCount * servCount];
}

Phase::Phase(const Phase& obj)
{
    serversCount = obj.serversCount;
    p_mu_stat = obj.p_mu_stat;
    p_n_stat = new int[serversCount * serversCount];
    for (uint_fast64_t k = 0; k < serversCount * serversCount; k++) {
        p_n_stat[k] = obj.p_n_stat[k];
    }
}

Phase& Phase::operator=(const Phase& right)
{
    //проверка на самоприсваивание
    if (this == &right) {
        return *this;
    }
    serversCount = right.serversCount;
    p_mu_stat = right.p_mu_stat;
    p_n_stat = new int[serversCount * serversCount];
    for (uint_fast64_t k = 0; k < serversCount * serversCount; k++) {
        p_n_stat[k] = right.p_n_stat[k];
    }
}
Phase::~Phase()
{
    delete[] p_n_stat;
    p_n_stat = NULL;
}