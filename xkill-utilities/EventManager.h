#pragma once

#include <vector>

#include "dllUtilities.h"

class IObserver;
enum EventType;
class Event;

/// Redirect events to each appropriate IObserver.
/** 
\ingroup utilities
\ingroup events
*/

class DLL_U EventManager
{
private:
	std::vector<std::vector<IObserver*>>* event_queues;

	EventManager();
	~EventManager();


public:
	static EventManager* getInstance();

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
};