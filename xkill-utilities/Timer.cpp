#include "Timer.h"

Timer::Timer()
{
	startTime_ = 0.0f;
	resetTimer();
}

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

void Timer::zeroTimer()
{
	timeLeft_ = 0.0f;
}

bool Timer::hasTimerExpired()
{
	return timeLeft_ <= 0.0f;
}

float Timer::getStartTime()
{
	return startTime_;
}

float Timer::getTimeLeft()
{
	return timeLeft_;
}