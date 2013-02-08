#pragma once

#include "dllUtilities.h"

class DLL_U Timer
{
public:
	Timer(float startTime);
	~Timer();

	void update(float delta);

	void setStartTime(float time);
	void resetTimer();
	bool hasTimerExpired();

private:
	float timeLeft_;
	float startTime_;
};