#pragma once

#include "EventType.h"
#include <vector>
#include <iostream>
using namespace std;

//
// Convenient delete macro
//

template<class T> void SafeDelete(T*& p)
{ 
	// NOTE: deleting a NULL-pointer is safe,
	// no check is needed
	delete p;
	p = NULL;
}

class IObserver
{
public:
	virtual void onEvent(Event* e) = 0;
};

class EventManager
{
private:
	vector<vector<IObserver*>> event_queues;

	EventManager()
	{
		// Build vectors with all events
		for(int i=0; i<EVENT_LAST; i++)
		{
			vector<IObserver*> v;
			event_queues.push_back(v);
		}
	}


public:
	static EventManager* getInstance()
	{
		static EventManager instance;
		return &instance;
	}

	void addObserver(IObserver* o, EventType type)
	{
		int index = type;
		event_queues[index].push_back(o);

		cout << "EVENTMANAGER: New subscriber on Events of Enum " << index << endl;
	}

	// Adds observer to every event queue,
	// observers shouldn't waste traffic by
	// subscribing to events they're not using
	// so this should only be used for testing purposes
	void addObserverToAll(IObserver* o)
	{
		for(int i=0; i<(int)event_queues.size(); i++)
		{
			event_queues[i].push_back(o);
		}
		cout << "EVENTMANAGER: New subscriber on Events of All Enum " << endl;
	}

	// Removes all instances of Observer
	void removeObserver(IObserver* observer)
	{
		for(int i=0; i<(int)event_queues.size(); i++)
			removeObserver(observer, (EventType)i);
	}

	// Removes instance of Observer from a specific event
	// NOTE: Order is not important, uses "swap trick" 
	// to swap "last element" with "removed"
	// This gives O(n) performance for remove instead of O(n^2)
	void removeObserver(IObserver* observer, EventType type)
	{
		int index = type;
		for(int i=0; i<(int)event_queues[index].size(); i++)
		{
			// TRUE: Element matches index; erase at index
			if(event_queues[index][i] == observer)
			{
				// remove using "swap trick"
				event_queues[index][i] = event_queues[index].back();
				event_queues[index].pop_back();

				// avoids unnecessary testing
				break;
			}
		}
	}

	// Send event to all relevant observers
	void sendEvent(Event* e)
	{
		int index = e->getType();
		cout << "EVENTMANAGER: Sends Event of Enum " << index << endl;
		for each(IObserver* observer in event_queues[index])
		{
			observer->onEvent(e);
		}
	}
};