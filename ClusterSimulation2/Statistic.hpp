#pragma once
#include <cstdint>
#include <vector>
#include "DynamicArray.hpp"


class Phase
{
private:
    double zero;
    // ���������� ��������
    uint8_t servCount;
    // ������ ������� �������
    unsigned int elSize;
    // ���������� ��������� � �������
    unsigned int arrSize;
    // ������
    double** p_n_stat;
    // � ��� ��������.
    void deleteArr();

    // ���������� � �������
    inline static unsigned int pow(unsigned int a, uint8_t p);

public:
    explicit Phase(uint8_t serversCount);
    Phase();
    ~Phase();

    // �����������  �����������
    Phase(const Phase& obj);

    // �������� �������������
    Phase& operator=(const Phase& right);

    // ������ � ���������
    inline double& operator()(unsigned int x, unsigned int y);
    // ������ ���������
    void print();

    // ���������� ������� � ������� �� ������� ���������
    void addTime(uint8_t* arr, unsigned int x, double time);

    // ���������� ��������� � ������ �������
    unsigned int getColumnSize() const;

    // ���������� ��������� � �������
    unsigned int size() const;
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
    
    // ���������� �������� � ��������
    uint64_t servers;

    // ������ ������������ ��� �������
    DynamicArraySimple<double, 100000000>* PqLow;
    DynamicArraySimple<double, 100000000>* PqHigh;

    // ���������� ���������� � ����������� ���������
    void normalizePointers();
public:
	Statistic();
    Statistic(unsigned int intervalCount, uint8_t servCount);
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

    // ����� ������ �������� ����� ������ � �������
    const double* getMeanAppsInQueue() const;

    const double* getMeanAppsInSystem() const;
    const double* getMeanProcessingTime() const;
    const double* getMeanWaitingTime() const;
    
    // �������
    const double* getMeanPower() const;
    void calculatePower(double e_0, double e_l, double e_h);

    // ������ �������� ���������
    uint64_t p;

    // ������������� ��������� �������. ��������� ������� ����������� ��������� �������:
    /* �� �������� - ���������� ������ � �������. ��������� �������:
    * P(x) - ����������� ����, ��� � ������� i ������
    * n_i_k - ����������� ����, ��� i-� ��������� ������ ��������� k ��������.
    */
    DynamicArraySimple<double, 100000000>* p_x_stat;
    // ������������� ���������� ��������, ������� ��������� i-� ��������� ������
    
    Phase* p_phase_stat[2];

};
