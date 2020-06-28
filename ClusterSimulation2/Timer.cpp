#include "Timer.hpp"
#include <iostream>

using namespace std;

Timer::Timer(double speed)
{
	events = new deque<Event>();
	id = 0;
	time = 0;
	sumTime = 0;
	this->speed = speed;
}

Timer::~Timer()
{
	delete events;
}

// Добавляет новое событие в очередь.
void Timer::addEvent(double time, uint64_t ID)
{
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

// Переход к следующему событию
bool Timer::nextEvent()
{
	if (!events->empty()) {
		// Поскольку список всегда упорядочен, то самый первый его элемент 
		// будет с наименьшим временем
		auto min = events->front();
		// Сместимся к этому событию
		time = min.time;
		sumTime = min.timeSum + time;
		id = min.id;
		events->pop_front();
		// Пересчитаем время для оставшихся событий
		for (auto i = events->begin(); i != events->end(); i++) {
			i->time -= time;
			i->timeSum += time;
		}
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
		// Указатель на событие прибытия новой заявки
		deque<Event>::iterator arrival;
		double time;
		// Пересчитаем время для событий
		for (auto i = events->begin(); i != events->end(); i++) {
			// Время пересчитываем, только если это уход заявки из системы
			if (i->id != 0) {
				i->time = i->time * speed / newSpeed;
			} else{
				arrival = i;
				time = i->time;
			}
		}
		events->erase(arrival);
		addEvent(time, 0);
	}
	
	speed = newSpeed;
}

//  Сколько ещё событий в списке
uint64_t Timer::size()
{
	return events->size();
}

// Для отладки
void Timer::printAll()
{
	for (auto i = events->begin(); i != events->end(); i++) {
		std::cout << i->time << "\t";
	}
	std::cout << endl;
}