#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

class TimeSystem
{
public:
	TimeSystem() {mTotalTime=0.0f;};
	virtual ~TimeSystem() {};

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual float GetDelta() = 0;

	void UpdateTotalTime(float deltaTime){mTotalTime+=deltaTime;}
	float GetTotalTimeInSeconds(){return mTotalTime;};

private:
	float mTotalTime;

	virtual void init() = 0;
};

#endif
