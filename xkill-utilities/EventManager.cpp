#pragma once
#include "EventManager.h"

#include <vector>
#include <iostream>
#include "IObserver.h"


EventManager::EventManager()
{
	// Build vectors with all events
	subscibers = new std::vector<std::vector<IObserver*>>;
	for(int i=0; i<EVENT_LAST; i++)
	{
		std::vector<IObserver*> v;
		subscibers->push_back(v);
	}

	// Build vectors with all events
	queues = new std::vector<std::vector<Event*>>;
	for(int i=0; i<EVENT_LAST; i++)
	{
		std::vector<Event*> v;
		queues->push_back(v);
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
	(*subscibers)[index].push_back(o);

	std::cout << "EVENTMANAGER: New subscriber on Events of Enum " << index << std::endl;
}

void EventManager::addObserverToAll(IObserver* o)
{
	for(int i=0; i<(int)subscibers->size(); i++)
	{
		(*subscibers)[i].push_back(o);
	}
	std::cout << "EVENTMANAGER: New subscriber on Events of All Enum " << std::endl;
}

void EventManager::removeObserver(IObserver* observer)
{
	for(int i=0; i<(int)subscibers->size(); i++)
		removeObserver(observer, (EventType)i);
}

void EventManager::removeObserver(IObserver* observer, EventType type)
{
	int index = type;
	for(int i=0; i<(int)subscibers[index].size(); i++)
	{
		// TRUE: Element matches index; erase at index
		if((*subscibers)[index][i] == observer)
		{
			// remove using "swap trick"
			subscibers[index][i] = subscibers[index].back();
			subscibers[index].pop_back();

			// avoids unnecessary testing
			break;
		}
	}
}


void EventManager::sendEvent(Event* e)
{
	int index = e->getType();
	//std::cout << "EVENTMANAGER: Sends Event of Enum " << index << std::endl;
	for(int i=0; i < (int)(*subscibers)[index].size(); i++)
	{
		(*subscibers)[index][i]->onEvent(e);
	}
}

EventManager::~EventManager()
{
	delete subscibers;
	delete queues;
}

void EventManager::queueEvent( Event* e )
{
	int index = e->getType();
	(*queues)[index].push_back(e);
}

void EventManager::flushQueuedEvents( EventType type )
{
	// send events
	int index = type;
	for(unsigned i=0; i<(*queues)[index].size(); i++)
	{
		sendEvent((*queues)[index][i]);
	}

	// free memory
	for(unsigned i=0; i<(*queues)[index].size(); i++)
	{
		delete (*queues)[index][i];
	}
	(*queues)[index].clear();
}

void EventManager::cleanAllQueues()
{
	// NOT IMPLEMENTED YET
}


