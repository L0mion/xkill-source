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
	
	_minClock = INT_MAX;
	_maxClock = 0;
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

	// Find min max values
	if(clocksElapsed > _maxClock)
		_maxClock = clocksElapsed;
	if(clocksElapsed < _minClock)
		_minClock = clocksElapsed;

	_totalElapsedClocks += clocksElapsed;
	_numSamples++;
	

	// Check to see if we have run long enough to send our measurement to listeners
	double elapsedSeconds = double(end - _lastMeasurement) / CLOCKS_PER_SEC;
	if(elapsedSeconds > 2.5)
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
	int msMin = int((double(_minClock) / CLOCKS_PER_SEC) * 1000);
	int msMax = int((double(_maxClock) / CLOCKS_PER_SEC) * 1000);
	

	std::string str_ms = Converter::IntToStr(ms);
	std::string str_msMin = Converter::IntToStr(msMin);
	std::string str_msMax = Converter::IntToStr(msMax);
	//std::string message = _timerName + ": "+ str_msMin +" [min "+ str_ms +" max "+ str_msMax +"] (ms)" ;
	std::string message = _timerName + ": "+ str_msMin +" < "+ str_ms +" < "+ str_msMax +" (ms)" ;
	SEND_EVENT(&Event_PostDebugMessage(_id, message));
}

int StopWatch::getUniqueId()
{
	static int numInstances = 0;
	int id = numInstances;
	numInstances++;

	return id;
}
