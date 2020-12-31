#pragma once
#include <cstdint>
#include <deque>

struct Event
{
    // Сколько времени осталось до события
    double time = 0;
    // ID события
    uint8_t id = 0;
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

    // Добавляет новое событие в очередь.
    void addEvent(double time, uint8_t ID);
    // Переход к следующему событию
    bool nextEvent();
    // Сколько времени прошло с преыдущего события
    double passedTime() const;
    // Сколько времени всего прошло с момента поступления первого события для этого события
    double timeSum() const;
    // id события
    uint8_t eventId() const;
    // функция для пересчёта времени
    void recalculateTime(double newSpeed);
    //  Сколько ещё событий в списке
    size_t size() const;
    // Печать всех событий (для отладки)
    void printAll();

    // Вернёт массив с количеством серверов, которое требуют старейшие заявки.
    uint8_t* getOldestApps(uint8_t& appsCount);
};