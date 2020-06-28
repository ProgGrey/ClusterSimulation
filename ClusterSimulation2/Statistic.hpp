#pragma once
#include <cstdint>
#include "DynamicArray.hpp"

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
    // Индекс текущего элемента
    uint64_t p;
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
    Statistic(int64_t intervalCount);
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
};
