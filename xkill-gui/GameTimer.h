#ifndef GAMETIMER_H
#define GAMETIMER_H

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
	GameTimer();
	~GameTimer();

	float getGameTime() const;
	float getDeltaTime() const;
	float getTotalTime();

	void reset();

	void start();
	void stop();

	void tick();
};

#endif