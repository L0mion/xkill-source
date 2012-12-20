#include "WindowsTime.h"

WindowsTime::WindowsTime()
{
	init();
}

WindowsTime::~WindowsTime()
{

}

void WindowsTime::Start()
{
	//mStart = clock();
	QueryPerformanceCounter((LARGE_INTEGER*)&mPrevTimeStamp);
}

void WindowsTime::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTimeStamp);
	//mStop = clock();
}

float WindowsTime::GetDelta()
{
	//return ((float)(mStop - mStart)/CLOCKS_PER_SEC);
	return (mCurrTimeStamp - mPrevTimeStamp) * mSecsPerCnt;
}

void WindowsTime::init()
{
	//mStart = 0.0f;
	//mStop = 0.0f;

	mCntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&mCntsPerSec);
	mSecsPerCnt = 1.0f / (float)mCntsPerSec;

	mPrevTimeStamp = 0;
	mCurrTimeStamp = 0;
}