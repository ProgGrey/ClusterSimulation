#pragma once
#include <cstdint>
#include "DynamicArray.hpp"

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
    /* ����������� ���� ���, � ������� n ������;
    * ����������� ����, ��� �������������� 1 ������������� ������ (��������� � ������� ����� ������������� ��� ������� �����);
    * ����������� ����, ��� �������������� 2 ������������� ������ (����� ��������� � ������� - ������);
    * ����������� ����, ��� �������������� ������������� ������ (����� ��������� � ������� - ������).
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
