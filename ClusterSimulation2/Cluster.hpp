#pragma once
#include "Timer.hpp"
#include "Statistic.hpp"
#include <deque>
#include "xoshiro/ExponentialDistribution.hpp"
#include "ProbabilityTest.hpp"

struct Application
{
    // ��������� ����� ������ ������
    double time = 0;
    // ������� ����� �����
    uint64_t cores;
};


class Cluster
{
private:
    // ���������� ��������� �����
    // ���� ����������� ������������� � ������ �����
    ProbabilityTest probTest;
    // ���������� ������� �� ������� � �������
    ExponentialDistribution timeArrival;

    // ���������� ������� �� ����� �� �������
    ExponentialDistribution timeLeave;

    // ������� ������, ��������� � �������
    std::deque<Application>* queue;

    // ������������-�� ���������?
    bool isWarming = false;
    // ������, ������� �����������, ����� ���������� ����������� �������
    // ���� id ������� ����� ����, �� ��� ������ ����� ������ � �������.
    // ���� id ������ ����, �� ��� ���� ������������ ������, � id ����� 
    // ���������� ��������, ������� �������� ��� ������ ��� ���������
    Timer* eventsB;

    // ����������� �������
    void applicationArrival();
    void applicationLeave();
    // ������� �������
    void applicationProcessing();
    void toLowMode();
    void toHighMode();

    // ��������� ���������
    double lambda;
    double mu_h;
    double mu_l;
    double p_h;
    double p_l;
    uint64_t servers;
    double* p;
    // ������� �������� ��������
    uint64_t freeServers;

    // ��������� ���-�� ��������, ����������� ������
    xoshiro256plus coresGen;
    uint64_t calculateCores();

    // ������� ����������� ������
    void calculateMetrics();

    // ��������� �� ������ � ����������������� ������
    bool isLowMode;

    // ����������
    Statistic *stat;

public:
    Cluster();
    Cluster(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint64_t servers, const double* p);
    ~Cluster();

    // ������ ���������
    // ������ �������� - �������: ������� �� SimCount �� �������� ����������
    void simulate(uint64_t warmingCount, uint64_t simCount, uint64_t intervalsCount);

    // �������� ������� ��������������
    //bool isStable();

    // ����� ������ �� �����. �����, � ������ �������, ��� �������.
    void printMetrics();
    
    // ��������� �� ������ �������� ����� ������ � ������� � ������ ������� �������
    const double* meanAppsInQueue;
    // ������� ����� ������ � �������
    const double* meanAppsInSystem;
    // ������� ����� ������������ ������
    const double* meanProcessingTime;
    // ������� ����� ���������� ������ � �������
    const double* meanWaitingTime;
    // ������� ������������ ��������. ����� ����������� ���������� ������� calculatePower
    const double* meanPower;

    // ������ ������������ ��������
    void calculatePower(double e_0, double e_l, double e_h);

    // ������������ �� �������������� ���������
    void useAlternativeGenertors();
    // ������������ �� �������� ���������.
    void useStandartGenertors();
    // ������������� ����������� ��������� �����
    void syncGenerators();
    // ������������� ���������
    void init(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint64_t servers, const double *p);
};