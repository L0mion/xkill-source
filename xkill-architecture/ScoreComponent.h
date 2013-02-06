#pragma once

#include <xkill-utilities/IObserver.h>

#include <vector>

struct Attribute_Player;

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
	std::vector<int> playerIndices_;		//! Used as a sorted list of playerAttributes according to 'priority'
	float currentSchedulerTime_;			//! Time until next scheduler selection
	float schedulerTime_;					//! Time between scheduler selection

	//! Sorts the vector using bubble sort
	/*!
	Sorts using bubble sort as it will be used to sort the scoreboard. This will likely to be
	a small array and also either sorted or almost sorted most of the time. 

	If it's to be used otherwise then it should probably be changed to use some other more 
	effective algorithm.
	*/
	void sort(std::vector<int>& elements);

	//! Swaps value between the 'first' and 'second' element in the array
	/*!
	\param elements The vector in which the elements should be swapped
	\param first First value to be swapped
	\param second Second value to swapped
	*/
	void swap(std::vector<int>& elements, int first, int second);
};