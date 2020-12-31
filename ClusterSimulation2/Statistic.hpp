#pragma once
#include <cstdint>
#include <vector>
#include "DynamicArray.hpp"


class Phase
{
private:
    double zero;
    // Количество серверов
    uint8_t servCount;
    // Размер столбца матрицы
    unsigned int elSize;
    // Количество элементов в векторе
    unsigned int arrSize;
    // массив
    double** p_n_stat;
    // и его удаление.
    void deleteArr();

    // Возведение в степень
    inline static unsigned int pow(unsigned int a, uint8_t p);

public:
    explicit Phase(uint8_t serversCount);
    Phase();
    ~Phase();

    // Конструктор  копирования
    Phase(const Phase& obj);

    // Оператор приравнивания
    Phase& operator=(const Phase& right);

    // Доступ к элементам
    inline double& operator()(unsigned int x, unsigned int y);
    // Печать состояний
    void print();

    // Вычисление позиции в массиве по вектору состояния
    void addTime(uint8_t* arr, unsigned int x, double time);

    // Количество элементов в столце матрицы
    unsigned int getColumnSize() const;

    // Количество элементов в массиве
    unsigned int size() const;
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

    // Приведение указателей к адекватному состоянию
    void normalizePointers();
public:
	Statistic();
    Statistic(unsigned int intervalCount, uint8_t servCount);
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

    // Вернёт вектор среднего числа заявок в очереди
    const double* getMeanAppsInQueue() const;

    const double* getMeanAppsInSystem() const;
    const double* getMeanProcessingTime() const;
    const double* getMeanWaitingTime() const;
    
    // Энергия
    const double* getMeanPower() const;
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
