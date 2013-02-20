#pragma once

#include "dllUtilities.h"
#include "EventType.h"
#include "EventManager.h"
#include "AttributeManager.h"
#include "FiniteState.h"

#include <vector>


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

// Gets the internal pointer holding the queued events.
#define GET_POINTER_TO_QUEUED_EVENTS(EventType)						\
	EventManager::getInstance()->getPointerToQueuedEvents(EventType);

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
	AttributeManager::instance()

// Fetches a vector<AttributeType>* of a specific Attribute
// from AttributeManager.
#define GET_ATTRIBUTES(AttributePointer)							\
	&(AttributeManager::instance())->AttributePointer.attributes;


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
#define BULLETPHYSICSDEBUGDRAW false //render Bullet Physics debug lines true/false
#define DEBUGPRINT(dataStream)
#endif
// Defines DEBUGPRINT to a cout function in Debug configuration
#ifdef XKILL_DEBUG
#define BULLETPHYSICSDEBUGDRAW true //render Bullet Physics debug lines true/false
#include <sstream>
#define DEBUGPRINT(dataStream)									\
	{                                                               \
	std::ostringstream oss;										\
	oss << dataStream << std::endl;								\
	printf(oss.str().c_str());									\
	}
#endif

// END OF EVIL