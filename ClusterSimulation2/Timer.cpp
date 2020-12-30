#include "Timer.hpp"
#include <iostream>

using namespace std;

Timer::Timer(double speed)
{
	events = new deque<Event>();
	id = 0;
	time = 0;
	sumTime = 0;
	timeToArrival = -1;
	this->speed = speed;
}

Timer::~Timer()
{
	delete events;
}

// Добавляет новое событие в очередь.
void Timer::addEvent(double time, uint64_t ID)
{
	if (ID == 0) {
		timeToArrival = time;
	}
	else {
		Event tmp;
		tmp.id = ID;
		tmp.time = time;
		// Вставляем событие в список так, чтобы он всегда был упорядочен
		auto it = events->begin();
		while ((it != events->end()) && (it->time < time)) {
			it++;
		}
		events->emplace(it, tmp);
	}
}

// Переход к следующему событию
bool Timer::nextEvent()
{
	if ((!events->empty()) || (timeToArrival >= 0)) {
		if (!events->empty()) {
			// Найдём событие ухода с минимальным временем
			// Поскольку список всегда упорядочен, то самый первый его элемент 
		    // будет с наименьшим временем
			auto min = events->front();
			// сравним с событием прихода
			if ((timeToArrival >= 0) && (min.time > timeToArrival)) {
				// Сместимся к этому событию
				time = timeToArrival;
				sumTime = 0;
				id = 0;
				timeToArrival = -1;
			} else {
				// Сместимся к этому событию
				time = min.time;
				sumTime = min.timeSum + time;
				id = min.id;
				events->pop_front();
			}
		} else if ((events->empty()) && (timeToArrival >= 0)){
			time = timeToArrival;
			sumTime = 0;
			id = 0;
			timeToArrival = -1;
		}
		// Пересчитаем время для оставшихся событий
		for (auto i = events->begin(); i != events->end(); i++) {
			i->time -= time;
			i->timeSum += time;
		}
		timeToArrival -= time;
		return true;
	}
	else {
		return false;
	}
}

// Функции, возвращающие текущее состояние времени и id события
double Timer::passedTime()
{
	return time;
}
double Timer::timeSum()
{
	return sumTime;
}

uint64_t Timer::eventId() {
	return id;
}

// функция для пересчёта времени
void Timer::recalculateTime(double newSpeed)
{
	if (!events->empty()) {
		// Пересчитаем время для событий
		for (auto i = events->begin(); i != events->end(); i++) {
			// Время пересчитываем, только если это уход заявки из системы
			i->time = i->time * speed / newSpeed;
		}
	}
	
	speed = newSpeed;
}

//  Сколько ещё событий в списке (включая текущее ушедшее)
uint64_t Timer::size()
{
	if (id > 0) {
		return events->size() + 1;
	} else {
		return events->size();
	}
	
}

// Для отладки
void Timer::printAll()
{
	for (auto i = events->begin(); i != events->end(); i++) {
		std::cout << i->time << "\t";
	}
	std::cout << endl;
}

uint64_t* Timer::getOldestApps(uint64_t& appsCount)
{
	if (id == 0) {
		appsCount = 0;
	} else{
		appsCount = 1;
		oldestApps[0] = id;
	}
	if (!events->empty()) {
		for (auto i = events->begin(); i != events->end(); i++) {
			oldestApps[appsCount] = i->id;
			appsCount++;
		}
	}
	return oldestApps;
}