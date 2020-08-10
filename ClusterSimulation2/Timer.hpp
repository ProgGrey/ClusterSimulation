#pragma once
#include <cstdint>
#include <deque>

struct Event
{
    // ������� ������� �������� �� �������
    double time;
    // ID �������
    uint64_t id;
    // �������� ������� ��� � ���������
    double timeSum = 0;
};

class Timer
{
private:
    uint64_t id;
    double time;
    double sumTime;
    std::deque<Event>* events;
    double speed;

    double timeToArrival;
public:
	Timer(double speed);
	~Timer();

    // ��������� ����� ������� � �������.
    void addEvent(double time, uint64_t id);
    // ������� � ���������� �������
    bool nextEvent();
    // ������� ������� ������ � ���������� �������
    double passedTime();
    // ������� ������� ����� ������ � ������� ����������� ������� ��� ����� �������
    double timeSum();
    // id �������
    uint64_t eventId();
    // ������� ��� ��������� �������
    void recalculateTime(double newSpeed);
    //  ������� ��� ������� � ������
    uint64_t size();
    // ������ ���� ������� (��� �������)
    void printAll();
};