#include "Cluster.hpp"
#include "Statistic.hpp"
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

    stat->p_x_stat[stat->p][(eventsB->size() + queue->size())] += eventsB->passedTime();


    // Вектора:
    if (isLowMode) {
        (*stat->PLow_p)[(eventsB->size() + queue->size())] += eventsB->passedTime();
        (*stat->PqLow_p)[queue->size()] += eventsB->passedTime();
    }else{
        (*stat->PHigh_p)[(eventsB->size() + queue->size())] += eventsB->passedTime();
        (*stat->PqHigh_p)[queue->size()] += eventsB->passedTime();
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
    // Новая статистика
    p_x_stat = NULL;
    p_n_stat[0] = NULL;
    p_n_stat[1] = NULL;
    normalizePointers();
}

Statistic::Statistic(int64_t intervalCount, uint64_t servCount)
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
    p_n_stat[0] = new DynamicArraySimple<double, 100000000>[intervalCount];
    p_n_stat[1] = new DynamicArraySimple<double, 100000000>[intervalCount];
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
    delete[] p_n_stat[0];
    delete[] p_n_stat[1];
    delete[] p_x_stat;
}

void Statistic::finalizeCalculation()
{
    // Среднее время обслуживания
    meanProcessingTime[p] /= leaveApplicationsCount;
    // Среднее время ожидания в очереди
    meanWaitingTime[p] /= applicationsInQueueCountSum;
    // Вектора для числа пользователей в очереди
    for (int k = 0; k < PqLow[p].size(); k++) {
        PqLow[p][k] /= simulationTime[p];
    }
    for (int k = 0; k < PqHigh[p].size(); k++) {
        PqHigh[p][k] /= simulationTime[p];
    }
    // Вектора для числа пользователей в системе
    for (int k = 0; k < PLow[p].size(); k++) {
        PLow[p][k] /= simulationTime[p];
    }
    for (int k = 0; k < PHigh[p].size(); k++) {
        PHigh[p][k] /= simulationTime[p];
    }
    
    // Матрицы новой статистики
    for (uint_fast64_t k = 0; k < p_n_stat[0][p].size(); k++) {
        p_n_stat[0][p][k] /= simulationTime[p];
    }
    for (uint_fast64_t k = 0; k < p_n_stat[1][p].size(); k++) {
        p_n_stat[1][p][k] /= simulationTime[p];
    }
    for (uint_fast64_t k = 0; k < p_x_stat[p].size(); k++) {
        p_x_stat[p][k] /= simulationTime[p];
    }
    // TODO: полностью перейти на новую статистику

    // Среднее число заявок в очереди
    meanAppsInQueue[p] = 0;
    for (int k = 0; k < PqLow[p].size(); k++) {
        meanAppsInQueue[p] += PqLow[p][k] * k;
    }
    for (int k = 0; k < PqHigh[p].size(); k++) {
        meanAppsInQueue[p] += PqHigh[p][k] * k;
    }
    // Среднее число заявок в системе
    meanAppsInSystem[p] = 0;
    for (int k = 0; k < p_x_stat[p].size(); k++) {
        meanAppsInSystem[p] += p_x_stat[p][k] * k;
    }
}

void Statistic::nextInterval()
{
    if (p < maxIntervalIndex) {
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
        p_n_stat[0][pp] = p_n_stat[0][p];
        p_n_stat[1][pp] = p_n_stat[1][p];
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


const double* Statistic::getMeanAppsInQueue()
{
    return meanAppsInQueue;
}

const double* Statistic::getMeanAppsInSystem()
{
    return meanAppsInSystem;
}
const double* Statistic::getMeanProcessingTime()
{
    return Statistic::meanProcessingTime;
}

const double* Statistic::getMeanWaitingTime()
{
    return meanWaitingTime;
}

const double* Statistic::getMeanPower()
{
    return meanPower;
}

void Statistic::calculatePower(double e_0, double e_l, double e_h)
{
    double p_0, p_l, p_h;
    // По всем интервалам
    for (uint_fast64_t k = 0; k <= maxIntervalIndex; k++) {
        p_0 = 0;
        p_l = 0;
        p_h = 0;
        if (PLow[k].size() > 0) {
            p_0 += PLow[k][0];
            for (uint_fast64_t i = 1; i < PLow[k].size(); i++) {
                p_l += PLow[k][i];
            }
        }
        if (PHigh[k].size() > 0) {
            p_0 += PHigh[k][0];
            for (uint_fast64_t i = 1; i < PHigh[k].size(); i++) {
                p_h += PHigh[k][i];
            }
        }
        meanPower[k] = p_0 * e_0 + p_l * e_l + p_h * e_h;
    }
}