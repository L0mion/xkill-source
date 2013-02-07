#pragma once

#include <xkill-utilities/IObserver.h>

#include <vector>

struct Attribute_Player;
template <class T> class AttributePtr;

//! A component to keep track of score
/*!

*/

class ScoreComponent : public IObserver
{
public:
	ScoreComponent();
	~ScoreComponent();

	virtual bool init();

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	float currentSchedulerTime_;						//! Time until next execution
	float schedulerTime_;								//! Time between executions
	float currentCycleTime_;							//! Time until next cycle
	float cycleTime_;									//!	Length of an cycle
	bool executionMode_;								//! Is the execution mode active
	unsigned int executingPlayerIndex_;					//! The index of the player that is currently executing

	int victoryScore_;									//! Score needed to win

	void schedulerScoreCounting(float delta);						//! Calculate the score according to scheduler game mode
	void deathMatchScoreCounting(float delta);						//!	Calculate the score according to death match game mode
};