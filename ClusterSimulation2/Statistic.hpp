#pragma once
#include <cstdint>
#include "DynamicArray.hpp"

class Phase {
private:
    uint64_t serversCount;
public:
    // Вероятность того, что система будет находится в указанном режиме работы
    // TODO: от этого избавимся, поскольку это равно сумме по всем p_n
    DynamicArraySimple<double, 100000000> p_mu_stat;

    // Первая координата - номер i последней заявки в системе
    // Вторая координата - требуемое этой заявке число серверов
    // Третья координата - число заявок в системе
    int *p_n_stat;

    // Оператор присваивания
    Phase& operator = (const Phase& right);
    // Конструктор копирования
    Phase(const Phase& obj);

    Phase();
    explicit Phase(uint64_t servCount);
    ~Phase();
};

class Statistic {
private:
    // Кол-во интервалов
    int64_t maxIntervalIndex;
    // Общее время симуляции
    double *simulationTime;
    // Среднее время обслуживания заявки
    double *meanProcessingTime;
    // Среднее время нахождения заявки в очереди
    double *meanWaitingTime;
    // Среднее число заявок в очереди
    double *meanAppsInQueue;
    // Среднее число заявок в системе
    double *meanAppsInSystem;
    // Средняя потребляемая мощность
    double* meanPower;
    
    // Количество серверов в кластере
    uint64_t servers;

    // Вектор вероятностей для очереди
    DynamicArraySimple<double, 100000000>* PqLow;
    DynamicArraySimple<double, 100000000>* PqHigh;

    // Вектор вероятностей для системы
    DynamicArraySimple<double, 100000000>* PLow;
    DynamicArraySimple<double, 100000000>* PHigh;

    // Приведение указателей к адекватному состоянию
    void normalizePointers();
public:
	Statistic();
    Statistic(int64_t intervalCount, uint64_t servCount);
	~Statistic();
    
    // Перейти к следующему интервалу
    void nextInterval();
    // Печать всей статистики. Нужна, в первую очередь, для отладки.
    void printAll();
    // Завершить вычисления для текущего интевала
    void finalizeCalculation();

    // Указатели на текущие позиции в статистике
    // Общее время симуляции
    double* simulationTime_p;
    // Среднее время обслуживания заявки
    double* meanProcessingTime_p;
    uint64_t leaveApplicationsCount;
    // Среднее время нахождения заявки в очереди
    double* meanWaitingTime_p;
    uint64_t applicationsInQueueCountSum;
    // Среднее число заявок в очереди
    double* meanAppsInQueue_p;
    // Среднее число заявок в системе
    double* meanAppsInSystem_p;

    // Вектор вероятностей для очереди
    DynamicArraySimple<double, 100000000>* PqLow_p;
    DynamicArraySimple<double, 100000000>* PqHigh_p;
    // Вектор вероятностей для системы
    DynamicArraySimple<double, 100000000>* PLow_p;
    DynamicArraySimple<double, 100000000>* PHigh_p;

    // Вернёт вектор среднего числа заявок в очереди
    const double* getMeanAppsInQueue();

    const double* getMeanAppsInSystem();
    const double* getMeanProcessingTime();
    const double* getMeanWaitingTime();
    
    // Энергия
    const double* getMeanPower();
    void calculatePower(double e_0, double e_l, double e_h);

    // Индекс текущего интервала
    uint64_t p;

    // распределение состояний системы. Состояния системы описываются следующим образом:
    /* По столбцам - количество заявок в системе. Структура столбца:
    * P(x) - вероятность того, что в системе i заявок
    * n_i_k - вероятность того, что i-я старейшая заявка потребует k серверов.
    */
    DynamicArraySimple<double, 100000000>* p_x_stat;
    // Распределение количества серверов, которое потребует i-я старейшая заявка
    
    Phase* p_phase_stat[2];

};
