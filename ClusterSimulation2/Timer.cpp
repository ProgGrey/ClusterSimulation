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

// ��������� ����� ������� � �������.
void Timer::addEvent(double time, uint64_t ID)
{
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

// ������� � ���������� �������
bool Timer::nextEvent()
{
	if (!events->empty()) {
		// ��������� ������ ������ ����������, �� ����� ������ ��� ������� 
		// ����� � ���������� ��������
		auto min = events->front();
		// ��������� � ����� �������
		time = min.time;
		sumTime = min.timeSum + time;
		id = min.id;
		events->pop_front();
		// ����������� ����� ��� ���������� �������
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
		// ��������� �� ������� �������� ����� ������
		deque<Event>::iterator arrival;
		double time;
		// ����������� ����� ��� �������
		for (auto i = events->begin(); i != events->end(); i++) {
			// ����� �������������, ������ ���� ��� ���� ������ �� �������
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

//  ������� ��� ������� � ������
uint64_t Timer::size()
{
	return events->size();
}

// ��� �������
void Timer::printAll()
{
	for (auto i = events->begin(); i != events->end(); i++) {
		std::cout << i->time << "\t";
	}
	std::cout << endl;
}