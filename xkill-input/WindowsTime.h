#ifdef _WIN32

#ifndef WINDOWS_TIME_H
#define WINDOWS_TIME_H

#include <Windows.h>
#include "TimeSystem.h"

class WindowsTime : public TimeSystem
{
public:
	WindowsTime();
	virtual ~WindowsTime();

	virtual void Start();
	virtual void Stop();
	virtual float GetDelta();

private:
	//clock_t mStart, mStop;
	__int64 mCntsPerSec;
	__int64 mPrevTimeStamp;
	__int64 mCurrTimeStamp;
	float mSecsPerCnt;

	virtual void init();
};

#endif

#endif