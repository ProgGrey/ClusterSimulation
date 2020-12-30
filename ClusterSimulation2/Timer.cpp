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

// ��������� ����� ������� � �������.
void Timer::addEvent(double time, uint64_t ID)
{
	if (ID == 0) {
		timeToArrival = time;
	}
	else {
		Event tmp;
		tmp.id = ID;
		tmp.time = time;
		// ��������� ������� � ������ ���, ����� �� ������ ��� ����������
		auto it = events->begin();
		while ((it != events->end()) && (it->time < time)) {
			it++;
		}
		events->emplace(it, tmp);
	}
}

// ������� � ���������� �������
bool Timer::nextEvent()
{
	if ((!events->empty()) || (timeToArrival >= 0)) {
		if (!events->empty()) {
			// ����� ������� ����� � ����������� ��������
			// ��������� ������ ������ ����������, �� ����� ������ ��� ������� 
		    // ����� � ���������� ��������
			auto min = events->front();
			// ������� � �������� �������
			if ((timeToArrival >= 0) && (min.time > timeToArrival)) {
				// ��������� � ����� �������
				time = timeToArrival;
				sumTime = 0;
				id = 0;
				timeToArrival = -1;
			} else {
				// ��������� � ����� �������
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
		// ����������� ����� ��� ���������� �������
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

// �������, ������������ ������� ��������� ������� � id �������
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

// ������� ��� ��������� �������
void Timer::recalculateTime(double newSpeed)
{
	if (!events->empty()) {
		// ����������� ����� ��� �������
		for (auto i = events->begin(); i != events->end(); i++) {
			// ����� �������������, ������ ���� ��� ���� ������ �� �������
			i->time = i->time * speed / newSpeed;
		}
	}
	
	speed = newSpeed;
}

//  ������� ��� ������� � ������ (������� ������� �������)
uint64_t Timer::size()
{
	if (id > 0) {
		return events->size() + 1;
	} else {
		return events->size();
	}
	
}

// ��� �������
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