#pragma once

#include <vector>

#include "dllUtilities.h"
#include "EventType.h"
#include "FiniteState.h"
#include "AttributeManager.h"

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
	std::vector<std::vector<IObserver*>>* subscibers;	
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
	Removes all queued messages without sending them,
	prevents memory leaks if not all messages is sent.
	*/
	void cleanAllQueues();
};


//
// EVIL MACROS
//

// Sends Event to relevant listeners.
#define SEND_EVENT(EventPointer)									\
EventManager::getInstance()->sendEvent(EventPointer);

// Queue an event so it can be sent later by using FLUSH MACRO.
// Memory deletion is handled by EventManager. 
#define QUEUE_EVENT(EventPointer)									\
EventManager::getInstance()->queueEvent(EventPointer);

// Sends all queued events to relevant listeners.
#define FLUSH_QUEUED_EVENTS(EventType)								\
EventManager::getInstance()->flushQueuedEvents(EventType);

// Subscribes a IObserver to events of EventType.
#define SUBSCRIBE_TO_EVENT(Subscriber,EventType)					\
EventManager::getInstance()->addObserver(Subscriber, EventType);

// Unsubscribes a IObserver to events of EventType.
#define UNSUBSCRIBE_TO_EVENT(Subscriber, EventType)					\
EventManager::getInstance()->removeObserver(Subscriber, EventType);

// Unsubscribes a IObserver to all events.
#define UNSUBSCRIBE_TO_EVENTS(Subscriber)							\
EventManager::getInstance()->removeObserver(Subscriber);

#define SUBSCRIBE_TO_EVENT(Subscriber,EventType)					\
EventManager::getInstance()->addObserver(Subscriber, EventType);

// Fetches a owners of a specific Attribute from AttributeManager
#define SHOW_MESSAGEBOX(Message)									\
{																	\
	Event_ShowMessageBox e(Message);								\
	EventManager::getInstance()->sendEvent(&e);						\
}

// TODO
#define ATTRIBUTE_MANAGER							\
	((AttributeManager*)AttributeManagerDLLWrapper::getInstance())

// Fetches a vector<AttributeType>* of a specific Attribute
// from AttributeManager.
#define GET_ATTRIBUTES(AttributePointer)							\
	&((AttributeManager*)AttributeManagerDLLWrapper::getInstance())->AttributePointer.attributes;

// Fetches a vector<AttributeType>* of a specific Attribute
// from AttributeManager.
#define GET_ATTRIBUTE_OWNERS(OwnerPointer)							\
	&((AttributeManager*)AttributeManagerDLLWrapper::getInstance())->OwnerPointer.owners;

// Fetches a owners of a specific Attribute from AttributeManager
#define GET_ENTITIES(EntityPointer)									\
{																	\
	Event_GetEntities e;											\
	EventManager::getInstance()->sendEvent(&e);						\
	EntityPointer = e.entities;										\
}

// Fetches a owners of a specific Attribute from AttributeManager
#define GET_STATE()													\
EventManager::getInstance()->state_TemporaryVariableUsedAsSubstituteForStateMachine								

// Defines DEBUGPRINT to an empty function in Release configuration
#ifndef XKILL_DEBUG
	#define DEBUGPRINT(dataStream)
#endif
// Defines DEBUGPRINT to a cout function in Debug configuration
#ifdef XKILL_DEBUG
	#include <iostream>
	#define DEBUGPRINT(dataStream)									\
	std::cout << dataStream << std::endl;			
#endif

// END OF EVIL