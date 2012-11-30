#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <windows.h>

class GameTimer
{
private:
	double secondsPerCount;
	double deltaTime;

	__int64 int_BaseTime;
	__int64 int_PausedTime;
	__int64 int_StopTime;
	__int64 int_PrevTime;
	__int64 int_CurrTime;

	bool Stopped;

public:
	GameTimer()
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


	float getGameTime()const
	{
		// If stopped, do not count the time that has passed since we stopped.
		if(Stopped)
			return (float)((int_StopTime-int_BaseTime)*secondsPerCount);
		else
			return (float)((int_CurrTime-int_PausedTime-int_BaseTime)*secondsPerCount);
	};
	float getDeltaTime()const
	{
		return (float)deltaTime;
	};
	float getTotalTime()
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

	void reset()
	{
		// Get current time
		__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t);

		int_BaseTime = t;
		int_PrevTime = t;
		int_StopTime = 0;
		Stopped  = false;
	};

	void start()
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
	};
	void stop()
	{
		if(!Stopped)
		{
			// Get current time
			__int64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t);

			int_StopTime = t;
			Stopped  = true;
		}
	};

	void tick()
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
	};


};

#endif