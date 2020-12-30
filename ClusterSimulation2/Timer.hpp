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
    // Сколько серверов требуют старейшие заявки.
    // Наша симуляция в любом случае не вытянет больше 
    // 15 серверов, поскольку описание состояния является
    // вектором размера n^n, где n - число серверов.
    uint64_t oldestApps[16];

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

    // Вернёт массив с количеством серверов, которое требуют старейшие заявки.
    uint64_t* getOldestApps(uint64_t& appsCount);
};