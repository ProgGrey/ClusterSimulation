#pragma once
#include <cstdint>
#include <deque>

struct Event
{
    // ������� ������� �������� �� �������
    double time;
    // ID �������
    uint8_t id;
    // �������� ������� ��� � ���������
    double timeSum = 0;
};

class Timer
{
private:
    // ������� �������� ������� ��������� ������.
    // ���� ��������� � ����� ������ �� ������� ������ 
    // 15 ��������, ��������� �������� ��������� ��������
    // �������� ������� n^n, ��� n - ����� ��������.
    uint8_t oldestApps[16];

    uint8_t id;
    double time;
    double sumTime;
    std::deque<Event>* events;
    double speed;

    double timeToArrival;
public:
	explicit Timer(double speed);
	~Timer();

    // ��������� ����� ������� � �������.
    void addEvent(double time, uint8_t ID);
    // ������� � ���������� �������
    bool nextEvent();
    // ������� ������� ������ � ���������� �������
    double passedTime() const;
    // ������� ������� ����� ������ � ������� ����������� ������� ������� ��� ����� �������
    double timeSum() const;
    // id �������
    uint8_t eventId() const;
    // ������� ��� ��������� �������
    void recalculateTime(double newSpeed);
    //  ������� ��� ������� � ������
    unsigned int size() const;
    // ������ ���� ������� (��� �������)
    void printAll();

    // ����� ������ � ����������� ��������, ������� ������� ��������� ������.
    uint8_t* getOldestApps(uint8_t& appsCount);
};