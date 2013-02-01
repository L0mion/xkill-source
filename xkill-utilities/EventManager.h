#pragma once

#include "Util.h"

class IObserver;
enum EventType;
class Event;

/// Redirect events to each appropriate IObserver.
/** 
\ingroup UTILITIES
\ingroup EVENTS
*/

class DLL_U EventManager
{
private:
	std::vector<std::vector<IObserver*>>* subscribers;	
	std::vector<std::vector<Event*>>* queues;		//! Used to queue Events if queue feature is used

	EventManager();
	~EventManager();

public:
	static EventManager* getInstance();

	// HACK: Used to access global game state
	StateType state_TemporaryVariableUsedAsSubstituteForStateMachine;

	void addObserver(IObserver* o, EventType type);

	/** 
	Adds observer to every event queue.
	Observers should not waste traffic by
	subscribing to events they are not using.
	Redirect events to each appropriate IObserver.
	*/
	void addObserverToAll(IObserver* o);

	/** 
	Removes all instances of Observer.
	*/
	void removeObserver(IObserver* observer);

	/** 
	Removes instance of Observer from a specific event.
	NOTE: Order is not important, uses "swap trick" 
	to swap "last element" with "removed"
	This gives O(n) performance for remove instead of O(n^2).
	*/
	void removeObserver(IObserver* observer, EventType type);

	/** 
	Sends event to all relevant observers.
	*/
	void sendEvent(Event* e);

	/** 
	Queue an event so it can be sent later by using flushEventQueue.
	*/
	void queueEvent(Event* e);

	/** 
	Sends queued Events
	*/
	void flushQueuedEvents(EventType type);

	/** 
	Get pointer to the internal vector holding the queued Events
	Must be Flushed afterwards to make sure events are deleted.
	*/
	std::vector<Event*>* getPointerToQueuedEvents(EventType type);

	/** 
	Removes all queued messages without sending them,
	prevents memory leaks if not all messages is sent.
	*/
	void cleanAllQueues();
};


