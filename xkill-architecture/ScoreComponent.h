#pragma once

#include <xkill-utilities/IObserver.h>

//#include <vector>
//struct Attribute_Player;
//template <class T> class AttributePtr;
class Timer;

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
	Timer* schedulerTimer_;								//! Keeps track of time left until next execution
	Timer* cycleTimer_;									//!	Keeps track of time left until next cycle
	bool executionMode_;								//! Is the execution mode active
	unsigned int executingPlayerIndex_;					//! The index of the player that is currently executing

	int victoryScore_;									//! Score needed to win

	void schedulerScoreCounting(float delta);						//! Calculate the score according to scheduler game mode
	void deathMatchScoreCounting(float delta);						//!	Calculate the score according to death match game mode
};