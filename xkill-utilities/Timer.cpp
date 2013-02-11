#include "Timer.h"

Timer::Timer(float startTime)
{
	startTime_ = startTime;
	resetTimer();
}

Timer::~Timer()
{

}

void Timer::update(float delta)
{
	timeLeft_ -= delta;
}

void Timer::setStartTime(float time)
{
	startTime_ = time;
}

void Timer::resetTimer()
{
	timeLeft_ = startTime_;
}

bool Timer::hasTimerExpired()
{
	return timeLeft_ <= 0.0f;
}