#pragma once

#include "Util.h"
#include "Converter.h"
#include <ctime>

class DLL_U StopWatch
{
private:
	clock_t _start;
	clock_t _lastMeasurement;

	clock_t _totalElapsedClocks;
	int _numSamples;

	int _id;
	std::string _timerName;


public:
	StopWatch(std::string timerName);

	void reset();
	void start();
	void stop();
	static int getUniqueId();
	void sendMeasurement();
};

// Sends Event to relevant listeners.
#define PROFILE_SECTION(HUD_NAME, CODE)								\
{																	\
	static StopWatch stopWatch(HUD_NAME);							\
	stopWatch.start();												\
	CODE														\
	stopWatch.stop();												\
}