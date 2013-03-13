#include "StopWatch.h"

StopWatch::StopWatch( std::string timerName )
{
	// Save name to easier identify timer
	_timerName = timerName;

	// Generate unique ID for each timer created
	_id = getUniqueId();

	reset();
}

void StopWatch::reset()
{
	_totalElapsedClocks = 0;
	_numSamples = 0;
	_lastMeasurement = clock();
	_start = _lastMeasurement;
}

void StopWatch::start()
{
	_start = clock();
}

void StopWatch::stop()
{
	// Save time measured
	clock_t end = clock();
	clock_t clocksElapsed = end - _start;
	_totalElapsedClocks += clocksElapsed;
	_numSamples++;

	// Check to see if we have run long enough to send our measurement to listeners
	double elapsedSeconds = double(end - _lastMeasurement) / CLOCKS_PER_SEC;
	if(elapsedSeconds > 0.5)
	{
		// Send event
		sendMeasurement();

		// Prepare for next measure
		reset();
	}
}

void StopWatch::sendMeasurement()
{
	// Convert time to milliseconds
	double seconds = double(_totalElapsedClocks / _numSamples) / CLOCKS_PER_SEC;
	int ms = int(seconds * 1000);

	std::string str_ms = Converter::IntToStr(ms);
	std::string message = _timerName + ": "+ str_ms +" (ms)";
	SEND_EVENT(&Event_PostDebugMessage(_id, message));
}

int StopWatch::getUniqueId()
{
	static int numInstances = 0;
	int id = numInstances;
	numInstances++;

	return id;
}
