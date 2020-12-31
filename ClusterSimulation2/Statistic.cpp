#include "Cluster.hpp"
#include "Statistic.hpp"
#include <omp.h>
#include <iostream>
using namespace std;


void Cluster::calculateMetrics()
{
    // Общее время симуляции
    *stat->simulationTime_p += eventsB->passedTime();
    // Среднее время обслуживания
    if (eventsB->eventId() > 0) {
        *stat->meanProcessingTime_p += eventsB->timeSum();
        stat->leaveApplicationsCount++;
    }

    // Распределение количества заявок в системе
    stat->p_x_stat[stat->p][(unsigned int)(eventsB->size() + queue->size())] += eventsB->passedTime();
    
    // Определим какой длины будет наш вектор, содержащий текущее состояние
    // Если сейчас наступило событие прихода, то число заявок в обработке будет равно eventsB->size(), поскольку добавится 
    // заявка в обработку может только после этой функции. Если это был уход, то число заявок в обработке равно 
    // eventsB->size() - 1. Но, с учётом только что ушедшей заявки их число равно eventsB->size().
    unsigned int vecLen = eventsB->size() + queue->size();
    if (vecLen > servers)
        vecLen = servers;
    // Запросим вектор текущих состояний у таймера. Этот вектор гарантировано будет нужной нам длины. Удалять его не надо.
    uint8_t AppsCountInVector;
    uint8_t* tmp = eventsB->getOldestApps(AppsCountInVector);
    // Доберём недостающих состояний из очереди
    uint8_t queuePos = 0;
    while (AppsCountInVector < vecLen) {
        tmp[AppsCountInVector] = (*queue)[queuePos].cores;
        AppsCountInVector++;
        queuePos++;
    }

    if (isLowMode)
    {
        stat->p_phase_stat[0][stat->p].addTime(tmp, eventsB->size() + queue->size(), eventsB->passedTime());
        stat->p_phase_stat[0][stat->p].p_mu_stat[(unsigned int)(eventsB->size() + queue->size())] += eventsB->passedTime();
        
    }else {
        stat->p_phase_stat[1][stat->p].addTime(tmp, eventsB->size() + queue->size(), eventsB->passedTime());
        stat->p_phase_stat[1][stat->p].p_mu_stat[(unsigned int)(eventsB->size() + (uint64_t)queue->size())] += eventsB->passedTime();
    }

    // Вектора:
    if (isLowMode) {
        (*stat->PLow_p)[(unsigned int)(eventsB->size() + queue->size())] += eventsB->passedTime();
        (*stat->PqLow_p)[(unsigned int)queue->size()] += eventsB->passedTime();
    }else{
        (*stat->PHigh_p)[(unsigned int)(eventsB->size() + queue->size())] += eventsB->passedTime();
        (*stat->PqHigh_p)[(unsigned int)queue->size()] += eventsB->passedTime();
    }
}

void Cluster::printMetrics()
{
    stat->printAll();
}


Statistic::Statistic()
{
    maxIntervalIndex = -1;
    simulationTime = NULL;
    meanProcessingTime = NULL;
    leaveApplicationsCount = 0;
    meanWaitingTime = NULL;
    applicationsInQueueCountSum = 0;
    meanAppsInQueue = NULL;
    meanAppsInSystem = NULL;
    PqLow = NULL;
    PqHigh = NULL;
    PLow = NULL;
    PHigh = NULL;
    meanPower = NULL;
    p = 0;
    servers = 1;
    // Новая статистика
    p_x_stat = NULL;
    p_phase_stat[0] = NULL;
    p_phase_stat[1] = NULL;
    normalizePointers();
}

Statistic::Statistic(unsigned int intervalCount, uint8_t servCount)
{
    this->maxIntervalIndex = intervalCount - 1;
    simulationTime = new double[intervalCount];
    meanProcessingTime = new double[intervalCount];
    leaveApplicationsCount = 0;
    meanWaitingTime = new double[intervalCount];
    applicationsInQueueCountSum = 0;
    meanAppsInQueue = new double[intervalCount];
    meanAppsInSystem = new double[intervalCount];
    meanPower = new double[intervalCount];
    PqLow = new DynamicArraySimple<double, 100000000>[intervalCount];
    PqHigh = new DynamicArraySimple<double, 100000000>[intervalCount];
    PLow = new DynamicArraySimple<double, 100000000>[intervalCount];
    PHigh = new DynamicArraySimple<double, 100000000>[intervalCount];

    servers = servCount;

    // Новая статистика
    p_phase_stat[0] = new Phase[intervalCount];
    p_phase_stat[1] = new Phase[intervalCount];
    for (int_fast64_t k = 0; k < intervalCount; k++) {
        p_phase_stat[0][k] = Phase(servCount);
        p_phase_stat[1][k] = Phase(servCount);
    }
    p_x_stat = new DynamicArraySimple<double, 100000000>[intervalCount];

    p = 0;
    if (intervalCount > 0) {
        simulationTime[0] = 0;
        meanProcessingTime[0] = 0;
        meanWaitingTime[0] = 0;
        meanAppsInQueue[0] = 0;
        meanAppsInSystem[0] = 0;
    }
    normalizePointers();
}

Statistic::~Statistic()
{
    delete[] simulationTime;
    delete[] meanProcessingTime;
    delete[] meanWaitingTime;
    delete[] meanAppsInQueue;
    delete[] meanAppsInSystem;
    delete[] PqLow;
    delete[] PqHigh;
    delete[] PLow;
    delete[] PHigh;
    delete[] meanPower;
    delete[] p_phase_stat[0];
    delete[] p_phase_stat[1];
    delete[] p_x_stat;
}

void Statistic::finalizeCalculation()
{
    // Среднее время обслуживания
    meanProcessingTime[p] /= leaveApplicationsCount;
    // Среднее время ожидания в очереди
    meanWaitingTime[p] /= applicationsInQueueCountSum;
    // Вектора для числа пользователей в очереди
    for (unsigned int k = 0; k < PqLow[p].size(); k++) {
        PqLow[p][k] /= simulationTime[p];
    }
    for (unsigned int k = 0; k < PqHigh[p].size(); k++) {
        PqHigh[p][k] /= simulationTime[p];
    }
    // Вектора для числа пользователей в системе
    for (unsigned int k = 0; k < PLow[p].size(); k++) {
        PLow[p][k] /= simulationTime[p];
    }
    for (unsigned int k = 0; k < PHigh[p].size(); k++) {
        PHigh[p][k] /= simulationTime[p];
    }
    
    // Матрицы новой статистики
    //p_phase_stat[0][p].print();
    //cout << "***" << endl;
    for (unsigned int  k = 1; k < p_x_stat[p].size(); k++) {
        for (unsigned int i = 0; i < p_phase_stat[0][p].getColumnSize(); i++) {
            p_phase_stat[0][p](k, i) /= (p_x_stat[p][k] == 0 ? 1 : p_x_stat[p][k]);
        }
        p_phase_stat[0][p].p_mu_stat[k] /= (p_x_stat[p][k] == 0 ? 1 : p_x_stat[p][k]);
    }
    //p_phase_stat[0][p].print();
    //cout << "####" << endl;
    //
    for (unsigned int  k = 1; k < p_x_stat[p].size(); k++) {
        for (unsigned int i = 0; i < p_phase_stat[1][p].getColumnSize(); i++) {
            p_phase_stat[1][p](k, i) /= (p_x_stat[p][k] == 0 ? 1 : p_x_stat[p][k]);
        }
        p_phase_stat[1][p].p_mu_stat[k] /= (p_x_stat[p][k] == 0 ? 1 : p_x_stat[p][k]);
    }
    for (unsigned int  k = 0; k < p_x_stat[p].size(); k++) {
        p_x_stat[p][k] /= simulationTime[p];
    }
    // TODO: полностью перейти на новую статистику

    // Среднее число заявок в очереди
    meanAppsInQueue[p] = 0;
    for (unsigned int k = 0; k < PqLow[p].size(); k++) {
        meanAppsInQueue[p] += PqLow[p][k] * k;
    }
    for (unsigned int k = 0; k < PqHigh[p].size(); k++) {
        meanAppsInQueue[p] += PqHigh[p][k] * k;
    }
    // Среднее число заявок в системе
    meanAppsInSystem[p] = 0;
    for (unsigned int k = 0; k < p_x_stat[p].size(); k++) {
        meanAppsInSystem[p] += p_x_stat[p][k] * k;
    }
}

void Statistic::nextInterval()
{
    if ((int64_t)p < maxIntervalIndex) {
        uint64_t pp = p + 1;
        // Копируем в новый интервал всю собранную к данному моменту статистику
        simulationTime[pp] = simulationTime[p];
        meanProcessingTime[pp] = meanProcessingTime[p];
        meanWaitingTime[pp] = meanWaitingTime[p];
        meanAppsInQueue[pp] = meanAppsInQueue[p];
        meanAppsInSystem[pp] = meanAppsInSystem[p];
        PqLow[pp] = PqLow[p];
        PqHigh[pp] = PqHigh[p];
        PLow[pp] = PLow[p];
        PHigh[pp] = PHigh[p];

        // Новая статистика:
        p_phase_stat[0][pp] = p_phase_stat[0][p];
        p_phase_stat[1][pp] = p_phase_stat[1][p];
        p_x_stat[pp] = p_x_stat[p];

        // Завершаем вычисление параметров
        finalizeCalculation();
        p++;
        normalizePointers();
    }
}

void printArray(const char *name, double* arr, uint64_t maxIndex)
{
    cout << name << "\t=\t";
    for (uint64_t k = 0; k < maxIndex; k++) {
        cout << arr[k] << '\t';
    }
    cout << arr[maxIndex] << endl;
}

void Statistic::printAll()
{
    printArray("simulationTime", simulationTime, maxIntervalIndex);
    printArray("meanProcessingTime", meanProcessingTime, maxIntervalIndex);
    printArray("meanWaitingTime", meanWaitingTime, maxIntervalIndex);
    printArray("meanAppsInQueue", meanAppsInQueue, maxIntervalIndex);
    printArray("meanAppsInSystem", meanAppsInSystem, maxIntervalIndex);
    //Альтернативные замеры для финального числа пользователей в очереди
    double tmp = 0;

    cout << "meanAppsInQueueAlt = " << tmp << endl;
}


void Statistic::normalizePointers()
{
    simulationTime_p = &simulationTime[p];
    meanProcessingTime_p = &meanProcessingTime[p];
    meanWaitingTime_p = &meanWaitingTime[p];
    meanAppsInQueue_p = &meanAppsInQueue[p];
    meanAppsInSystem_p = &meanAppsInSystem[p];
    PqHigh_p = &PqHigh[p];
    PqLow_p = &PqLow[p];
    PHigh_p = &PHigh[p];
    PLow_p = &PLow[p];
}


const double* Statistic::getMeanAppsInQueue()  const
{
    return meanAppsInQueue;
}

const double* Statistic::getMeanAppsInSystem() const
{
    return meanAppsInSystem;
}
const double* Statistic::getMeanProcessingTime() const
{
    return Statistic::meanProcessingTime;
}

const double* Statistic::getMeanWaitingTime() const
{
    return meanWaitingTime;
}

const double* Statistic::getMeanPower() const
{
    return meanPower;
}

void Statistic::calculatePower(double e_0, double e_l, double e_h)
{
    double p_l, p_h;
    // По всем интервалам
    for (int64_t k = 0; k <= maxIntervalIndex; k++) {
        p_l = 0;
        p_h = 0;
        for (unsigned int i = 1; i < p_phase_stat[0][k].p_mu_stat.size(); i++) {
            for (unsigned int j = 0; j < p_phase_stat[0][k].getColumnSize(); j++) {
                p_l += p_phase_stat[0][k](i, j) * p_x_stat[k][i];
            }
        }
        for (unsigned int i = 1; i < p_phase_stat[1][k].p_mu_stat.size(); i++) {
            for (unsigned int j = 0; j < p_phase_stat[1][k].getColumnSize(); j++) {
                p_h += p_phase_stat[1][k](i, j) * p_x_stat[k][i];
            }
        }
        meanPower[k] = p_x_stat[k][0] * e_0 + p_l * e_l + p_h * e_h;
    }
}