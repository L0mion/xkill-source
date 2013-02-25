#include "EventManager.h"

#include <vector>
#include "IObserver.h"

EventManager::EventManager()
{
	state_TemporaryVariableUsedAsSubstituteForStateMachine = STATE_MAINMENU;

	//
	// Build vectors with all events
	//

	// Subscribers
	subscribers = new std::vector<std::vector<IObserver*>>;
	for(int i=0; i<EVENT_LAST; i++)
	{
		std::vector<IObserver*> v;
		subscribers->push_back(v);
	}

	// Queues
	queues = new std::vector<std::vector<Event*>>;
	for(int i=0; i<EVENT_LAST; i++)
	{
		std::vector<Event*> v;
		queues->push_back(v);
	}
	
	// Delayed queues
	delayedQueues = new std::vector<std::vector<DelayedEvent>>;
	for(int i=0; i<EVENT_LAST; i++)
	{
		std::vector<DelayedEvent> v;
		delayedQueues->push_back(v);
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
	(*subscribers)[index].push_back(o);

	DEBUGPRINT("EVENTMANAGER: New subscriber on Events of Enum " << index);
}

void EventManager::addObserverToAll(IObserver* o)
{
	for(int i=0; i<(int)subscribers->size(); i++)
	{
		(*subscribers)[i].push_back(o);
	}
	DEBUGPRINT("EVENTMANAGER: New subscriber on Events of All Enum ");
}

void EventManager::removeObserver(IObserver* observer)
{
	for(int i=0; i<(int)subscribers->size(); i++)
		removeObserver(observer, (EventType)i);
}

void EventManager::removeObserver(IObserver* observer, EventType type)
{
	int index = type;
	for(int i=0; i<(int)subscribers->at(index).size(); i++)
	{
		// TRUE: Element matches index; erase at index
		if((*subscribers)[index][i] == observer)
		{
			// remove using "swap trick"
			(*subscribers)[index][i] = (*subscribers)[index].back();
			(*subscribers)[index].pop_back();

			// avoids unnecessary testing
			break;
		}
	}
}


void EventManager::sendEvent(Event* e)
{
	EventType eventTyp = e->getType();
	int index = eventTyp;
	//std::cout << "EVENTMANAGER: Sends Event of Enum " << index << std::endl;
	for(int i=0; i < (int)(*subscribers)[index].size(); i++)
	{
		(*subscribers)[index][i]->onEvent(e);
	}
}

EventManager::~EventManager()
{
	cleanAllQueues();

	delete subscribers;
	delete queues;
	delete delayedQueues;
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

std::vector<Event*>* EventManager::getPointerToQueuedEvents( EventType type )
{
	int index = type;
	return &queues->at(index);
}

void EventManager::cleanAllQueues()
{
	// Sends all remaining queued messages to prevent memory leak
	for(unsigned i=0; i<queues->size(); i++)
	{
		flushQueuedEvents((EventType)i);
	}

	// Delete all remaining delayed events
	for(unsigned i=0; i<delayedQueues->size(); i++)
	{
		for(unsigned j=0; j<delayedQueues->at(i).size(); j++)
		{
			Event* e = delayedQueues->at(i)[j].getEvent();
			delete e;
		}
	}
	delayedQueues->clear();
}

void EventManager::update( float delta )
{
	sendExpiredDelayedEvents(delta);
}

void EventManager::sendExpiredDelayedEvents( float delta )
{
	// Loop through each queue corresponding to each event
	// and send all events of that type
	for(unsigned i=0; i<delayedQueues->size(); i++)
	{
		for(unsigned j=0; j<delayedQueues->at(i).size(); j++)
		{
			DelayedEvent* delayedEvent = &delayedQueues->at(i)[j];

			if(delayedEvent->isReady(delta))
			{
				Event* e = delayedEvent->getEvent();

				sendEvent(e);

				// delete event
				delete e;

				// delete DelayedEvent from vector using swap-trick O(1)
				delayedQueues->at(i)[j] = delayedQueues->at(i).back();
				delayedQueues->at(i).pop_back();

				// repeat this index since it has been changed into an unvisited element
				j--; 
			}
		}
	}
}

void EventManager::postDelayedEvent( Event* e, float delay )
{
	int index = e->getType();
	delayedQueues->at(index).push_back(DelayedEvent(e, delay));
}



DelayedEvent::DelayedEvent( Event* e, float delay )
{
	this->e = e;
	this->delay = delay;
}

bool DelayedEvent::isReady( float delta )
{
	// Decrement timer
	if(delay > 0.0f)
		delay -= delta;

	// Check expiration condition
	if(delay <= 0.0f)
		return true;

	return false;
}

Event* DelayedEvent::getEvent()
{
	return e;
}
