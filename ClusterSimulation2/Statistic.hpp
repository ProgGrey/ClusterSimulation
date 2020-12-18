#pragma once
#include <cstdint>
#include "DynamicArray.hpp"

class StateDist{
    public:
    // ����������� ����, ��� � ������� i �������������
    double x_t;
    // ������������� ��� ������� �������� ��������. ��� ����������� ��������: 0, ��� ������������ - 1
    // � ����� �������� - ����������� ����, ��� ������ ������� n �������� ��� ��������� ��������
    double* n[2];

    //�����������
    StateDist(uint64_t serverCount);
    // ����������
    ~StateDist();
};

class Statistic {
private:
    // ���-�� ����������
    int64_t maxIntervalIndex;
    // ����� ����� ���������
    double *simulationTime;
    // ������� ����� ������������ ������
    double *meanProcessingTime;
    // ������� ����� ���������� ������ � �������
    double *meanWaitingTime;
    // ������� ����� ������ � �������
    double *meanAppsInQueue;
    // ������� ����� ������ � �������
    double *meanAppsInSystem;
    // ������� ������������ ��������
    double* meanPower;
    // ������ �������� ��������
    uint64_t p;


    // TODO: ���������� ����� ���������� ��������� �������: ���� ������������� ������ � ������� � ���� �������, �� ������� ������� (������ ��� ������������� �������):
    /* ��������� ������� ������������� ������������� Pi ={Pi_0, Pi_1, Pi_2, ...}.
    * Pi_i:
    * ����������� ����, ��� � ������� i ��������;
    * ����������� ����, ��� ��������� ������� 1 ������� �������� 1;
    * ����������� ����, ��� ��������� ������� 1 ������� �������� 2;
    * ����������� ����, ��� ��������� ������� 2 ������� �������� 1;
    * ����������� ����, ��� ��������� ������� 2 ������� �������� 2;
    * ����������� ����, ��� ������������� ������� 1 ������� �������� 1;
    * ����������� ����, ��� ������������� ������� 1 ������� �������� 2;
    * ����������� ����, ��� ������������� ������� 2 ������� �������� 1;
    * ����������� ����, ��� ������������� ������� 2 ������� �������� 2.
    */

    // ������ ������������ ��� �������
    DynamicArraySimple<double, 100000000>* PqLow;
    DynamicArraySimple<double, 100000000>* PqHigh;

    // ������ ������������ ��� �������
    DynamicArraySimple<double, 100000000>* PLow;
    DynamicArraySimple<double, 100000000>* PHigh;

    // ���������� ���������� � ����������� ���������
    void normalizePointers();
public:
	Statistic();
    Statistic(int64_t intervalCount);
	~Statistic();
    
    // ������� � ���������� ���������
    void nextInterval();
    // ������ ���� ����������. �����, � ������ �������, ��� �������.
    void printAll();
    // ��������� ���������� ��� �������� ��������
    void finalizeCalculation();

    // ��������� �� ������� ������� � ����������
    // ����� ����� ���������
    double* simulationTime_p;
    // ������� ����� ������������ ������
    double* meanProcessingTime_p;
    uint64_t leaveApplicationsCount;
    // ������� ����� ���������� ������ � �������
    double* meanWaitingTime_p;
    uint64_t applicationsInQueueCountSum;
    // ������� ����� ������ � �������
    double* meanAppsInQueue_p;
    // ������� ����� ������ � �������
    double* meanAppsInSystem_p;

    // ������ ������������ ��� �������
    DynamicArraySimple<double, 100000000>* PqLow_p;
    DynamicArraySimple<double, 100000000>* PqHigh_p;
    // ������ ������������ ��� �������
    DynamicArraySimple<double, 100000000>* PLow_p;
    DynamicArraySimple<double, 100000000>* PHigh_p;

    // ����� ������ �������� ����� ������ � �������
    const double* getMeanAppsInQueue();

    const double* getMeanAppsInSystem();
    const double* getMeanProcessingTime();
    const double* getMeanWaitingTime();
    
    // �������
    const double* getMeanPower();
    void calculatePower(double e_0, double e_l, double e_h);
};
