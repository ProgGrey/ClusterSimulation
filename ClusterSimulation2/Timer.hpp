#pragma once
#include <cstdint>
#include <deque>

struct Event
{
    // Сколько времени осталось до события
    double time;
    // ID события
    uint64_t id;
    // Склолько времени уже в обработке
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

    // Добавляет новое событие в очередь.
    void addEvent(double time, uint64_t id);
    // Переход к следующему событию
    bool nextEvent();
    // Сколько времени прошло с преыдущего события
    double passedTime();
    // Сколько времени всего прошло с момента поступления события для этого события
    double timeSum();
    // id события
    uint64_t eventId();
    // функция для пересчёта времени
    void recalculateTime(double newSpeed);
    //  Сколько ещё событий в списке
    uint64_t size();
    // Печать всех событий (для отладки)
    void printAll();
};