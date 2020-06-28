#include "Cluster.hpp"

using namespace std;

uint64_t Cluster::calculateCores()
{
    if (p != NULL) {
        // ����������� ������������ ��������
        double tmp = coresGen();
        tmp /= coresGen.max();
        for (uint64_t k = 0; k < servers - 1; k++){
            tmp -= p[k];
            if (tmp <= 0.0) {
                return k + 1;
            }
            return servers;
        }

    }
    return 1;
}

// ������ ����� ������ � �������
void Cluster::applicationArrival()
{
    // ������� ������ � ������� �� ���������
    Application tmp;
    tmp.time = 0;
    tmp.cores = calculateCores();
    queue->push_back(tmp);
    // ������� ������� �������� ����� ������
    eventsB->addEvent(timeArrival(), 0);
    applicationProcessing();
    // ��������� ����������� ������� � ���������� �����
    if ((isLowMode) && (probTest(p_h))) {
        toHighMode();
    }
}

// ���� ������ �� �������
void Cluster::applicationLeave()
{
    freeServers += eventsB->eventId();
    applicationProcessing();
    // ��������� ����������� ������� � ���������� �����
    if ((!isLowMode) && (probTest(p_l))) {
        toLowMode();
    }
}

// �������� ������ � ���������
void Cluster::applicationProcessing()
{
    if (queue->size() > 0) {
        Application tmp = queue->front();
        if (tmp.cores <= freeServers) {
            // �������� ������ �� ���������
            freeServers -= tmp.cores;
            eventsB->addEvent(timeLeave(), tmp.cores);
            queue->pop_front();
            // ������ ������� �� �������� ������� �������� � �������
            if (!isWarming) {
                *stat->meanWaitingTime_p += tmp.time;
                stat->applicationsInQueueCountSum++;
            }
            // ���������� ��������� ��� ���� ������ �� ���������
            applicationProcessing();
        }
    }
}

void Cluster::toLowMode()
{
    isLowMode = true;
    eventsB->recalculateTime(mu_l);
    timeLeave.lambda = mu_l;
}

void Cluster::toHighMode()
{
    isLowMode = false;
    eventsB->recalculateTime(mu_h);
    timeLeave.lambda = mu_h;
}

// ���������
void Cluster::simulate(uint64_t warmingCount, uint64_t simCount, uint64_t intervalsCount)
{
    // ������� ������� �������� ������ ������
    eventsB->addEvent(timeArrival(), 0);
    // �������. ������� �� ��������
    isWarming = true;
    for (uint64_t k = 0; k < warmingCount; k++) {
        // ������� � ���������� �������
        eventsB->nextEvent();
        // ����������� ����� �������� ������ � �������
        // �� �� ����� ������ ��� ������ �� ������ �� �������,
        // ����� �� ��������� ���������� ����� �� ��������
        for (auto i = queue->begin(); i != queue->end(); i++) {
            i->time += eventsB->passedTime();
        }
        // ��������� ��� ����� �������
        if (eventsB->eventId() == 0) {
            // � ������� ������� ����� ������
            applicationArrival();
        }
        else {
            // �� ������� ���� ������
            applicationLeave();
        }
    }
    isWarming = false;
    delete stat;
    stat = new Statistic(intervalsCount);
    // �������� ���������
    for (uint64_t i = 0; i < intervalsCount; i++) {
        for (uint64_t k = 0; k < simCount; k++) {
            // ������� � ���������� �������
            eventsB->nextEvent();
            // ����������� ����� �������� ������ � �������
            for (auto i = queue->begin(); i != queue->end(); i++) {
                i->time += eventsB->passedTime();
            }
            // ��������� �������
            calculateMetrics();
            // ��������� ��� ����� �������
            if (eventsB->eventId() == 0) {
                // � ������� ������� ����� ������
                applicationArrival();
            }
            else {
                // �� ������� ���� ������
                applicationLeave();
            }
        }
        // ����� �������� ���������� ��� ���������� ���������
        stat->nextInterval();
    }
    stat->finalizeCalculation();
    meanAppsInQueue = stat->getMeanAppsInQueue();
    meanAppsInSystem = stat->getMeanAppsInSystem();
    meanProcessingTime = stat->getMeanProcessingTime();
    meanWaitingTime = stat->getMeanWaitingTime();
    meanPower = stat->getMeanPower();
}

void Cluster::calculatePower(double e_0, double e_l, double e_h)
{
    stat->calculatePower(e_0, e_l, e_h);
}