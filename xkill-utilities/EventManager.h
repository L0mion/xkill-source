#pragma once

#include <vector>

#include "dllUtilities.h"
#include "EventType.h"

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


//
// EVIL MACROS
//

// Macro for sending Events
#define SEND_EVENT(EventPointer)									\
EventManager::getInstance()->sendEvent(EventPointer);

// Subscribes a IObserver to events of EventType.
#define SUBSCRIBE_TO_EVENT(Subscriber,EventType)					\
EventManager::getInstance()->addObserver(Subscriber, EventType);

// Fetches a owners of a specific Attribute from AttributeManager
#define SHOW_MESSAGEBOX(Message)									\
{																	\
	Event_showMessageBox e(Message);								\
	EventManager::getInstance()->sendEvent(&e);						\
}

// Fetches a vector<AttributeType>* of a specific Attribute
// from AttributeManager.
#define GET_ATTRIBUTES(AttributePointer, AttributeType, Enum)		\
{																	\
	Event_getAttribute e(Enum);										\
	EventManager::getInstance()->sendEvent(&e);						\
	AttributePointer = (std::vector<AttributeType>*)e.hostVector;	\
}

// Fetches a owners of a specific Attribute from AttributeManager
#define GET_ATTRIBUTE_OWNERS(OwnerPointer, Enum)					\
{																	\
	Event_getAttribute e(Enum);										\
	EventManager::getInstance()->sendEvent(&e);						\
	OwnerPointer = e.owners;										\
}

// END OF EVIL