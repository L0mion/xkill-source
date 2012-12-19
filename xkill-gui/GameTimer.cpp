#include "GameTimer.h"

#include <windows.h>

GameTimer::GameTimer()
{
	secondsPerCount=0.0f;
	deltaTime=-1.0;
	int_BaseTime=0;
	int_PausedTime=0;
	int_PrevTime=0;
	int_CurrTime=0;
	Stopped=false;

	// Convert counts to seconds
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount=1.0/(double)countsPerSec;
}

GameTimer::~GameTimer()
{
}

float GameTimer::getGameTime() const
{
	// If stopped, do not count the time that has passed since we stopped.
	if(Stopped)
		return (float)((int_StopTime-int_BaseTime)*secondsPerCount);
	else
		return (float)((int_CurrTime-int_PausedTime-int_BaseTime)*secondsPerCount);
}

float GameTimer::getDeltaTime() const
{
	return (float)deltaTime;
}

float GameTimer::getTotalTime()
{
	// Do not count time passed since stopped.
	if(int_StopTime)
	{
		return (float)(((int_StopTime - int_PausedTime)-int_BaseTime)*secondsPerCount);
	}
	else
	{
		return (float)(((int_CurrTime-int_PausedTime)-int_BaseTime)*secondsPerCount);
	}
}

void GameTimer::reset()
{
	// Get current time
	__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t);

	int_BaseTime = t;
	int_PrevTime = t;
	int_StopTime = 0;
	Stopped  = false;
}

void GameTimer::start()
{
	if(Stopped)
	{
		// Get current time
		__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t);

		int_PrevTime = t;
		int_PausedTime += (t-int_StopTime);	
		int_StopTime = 0;
		Stopped  = false;
	}
}

void GameTimer::stop()
{
	if(!Stopped)
	{
		// Get current time
		__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t);

		int_StopTime = t;
		Stopped  = true;
	}
};

void GameTimer::tick()
{
	// Ignore
	if(Stopped)
	{
		deltaTime = 0.0;
		return;
	}

	// Get current time
	__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t); int_CurrTime = t;

	// Compute "dt"
	deltaTime = (int_CurrTime-int_PrevTime)*secondsPerCount;

	// Save current time as previous time
	int_PrevTime = int_CurrTime;

	// Prevent scenario where "dt" can become negative 
	if(deltaTime < 0.0)
		deltaTime = 0.0;
}