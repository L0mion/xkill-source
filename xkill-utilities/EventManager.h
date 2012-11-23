#pragma once

#include <vector>

#include "dllUtilities.h"

class IObserver;
enum EventType;
class Event;
//
// Convenient delete macro
//



class DLL EventManager
{
private:
	std::vector<std::vector<IObserver*>>* event_queues;

	EventManager();


public:
	static EventManager* getInstance();

	void addObserver(IObserver* o, EventType type);

	// Adds observer to every event queue,
	// observers shouldn't waste traffic by
	// subscribing to events they're not using
	// so this should only be used for testing purposes
	void addObserverToAll(IObserver* o);

	// Removes all instances of Observer
	void removeObserver(IObserver* observer);

	// Removes instance of Observer from a specific event
	// NOTE: Order is not important, uses "swap trick" 
	// to swap "last element" with "removed"
	// This gives O(n) performance for remove instead of O(n^2)
	void removeObserver(IObserver* observer, EventType type);

	// Send event to all relevant observers
	void sendEvent(Event* e);
};