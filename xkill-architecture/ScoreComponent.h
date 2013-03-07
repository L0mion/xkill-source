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
	Timer* schedulerTimer_;						//! Keeps track of time left until next execution
	Timer* cycleTimer_;							//!	Keeps track of time left until next cycle
	Timer* gameTimer_;							//! Keeps track of current session time and when it's over
	bool executionMode_;						//! Is the execution mode active
	bool nullProcessExecuting_;					//! Is the null process executing
	unsigned int executingPlayerIndex_;			//! The index of the player that is currently executing
	int priorityWhenSelectedForExecution;		//! Store player priority of the player selected for execution. During execution the player may gain more priority than when selcted.

	void schedulerScoreCounting(float delta);	//! Calculate the score according to scheduler game mode
	void deathMatchScoreCounting(float delta);	//!	Calculate the score according to death match game mode

	void handleExecutionMode(float delta);		//! Handle an player executing
	void handleSchedulerMode(float delta);		//! Handle null process running and scheduler mode

	void activateNullProcess();					//! Activate null process
	void deactivateNullProcess();				//! Deactivate null process
	void executePlayer(int playerIndex);		//! Enter execution mode and execute the player with index 'playerIndex'
};