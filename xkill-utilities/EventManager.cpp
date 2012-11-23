#pragma once
#include "EventManager.h"

#include "EventType.h"
#include <vector>
#include <iostream>
#include "IObserver.h"


EventManager::EventManager()
	{
		event_queues = new std::vector<std::vector<IObserver*>>;
		// Build vectors with all events
		for(int i=0; i<EVENT_LAST; i++)
		{
			std::vector<IObserver*> v;
			event_queues->push_back(v);
		}
	}


EventManager* EventManager::getInstance()
{
	static EventManager instance;
	return &instance;
}

void EventManager::addObserver(IObserver* o, EventType type)
{
	int index = type;
	(*event_queues)[index].push_back(o);

	std::cout << "EVENTMANAGER: New subscriber on Events of Enum " << index << std::endl;
}

	// Adds observer to every event queue,
	// observers shouldn't waste traffic by
	// subscribing to events they're not using
	// so this should only be used for testing purposes
void EventManager::addObserverToAll(IObserver* o)
{
	for(int i=0; i<(int)event_queues->size(); i++)
	{
		(*event_queues)[i].push_back(o);
	}
	std::cout << "EVENTMANAGER: New subscriber on Events of All Enum " << std::endl;
}

	// Removes all instances of Observer
void EventManager::removeObserver(IObserver* observer)
{
	for(int i=0; i<(int)event_queues->size(); i++)
		removeObserver(observer, (EventType)i);
}

	// Removes instance of Observer from a specific event
	// NOTE: Order is not important, uses "swap trick" 
	// to swap "last element" with "removed"
	// This gives O(n) performance for remove instead of O(n^2)
void EventManager::removeObserver(IObserver* observer, EventType type)
{
	int index = type;
	for(int i=0; i<(int)event_queues[index].size(); i++)
	{
		// TRUE: Element matches index; erase at index
		if((*event_queues)[index][i] == observer)
		{
			// remove using "swap trick"
			event_queues[index][i] = event_queues[index].back();
			event_queues[index].pop_back();

			// avoids unnecessary testing
			break;
		}
	}
}


void EventManager::sendEvent( Event* e )
{
	int index = e->getType();
	std::cout << "EVENTMANAGER: Sends Event of Enum " << index << std::endl;

	for(int i=0; i<(int)event_queues[index].size(); i++)
	{
		(*event_queues)[index][i]->onEvent(e);
	}
}


