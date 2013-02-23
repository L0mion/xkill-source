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
#define SEND_EVENT(EVENT_POINTER)									\
	EventManager::getInstance()->sendEvent(EVENT_POINTER);

// Queue an event so it can be sent later by using FLUSH MACRO.
// Memory deletion is handled by EventManager. 
#define QUEUE_EVENT(EVENT_POINTER)									\
	EventManager::getInstance()->queueEvent(EVENT_POINTER);

// Sends all queued events to relevant listeners.
#define FLUSH_QUEUED_EVENTS(EVENT_TYPE)								\
	EventManager::getInstance()->flushQueuedEvents(EVENT_TYPE);

// Sends all queued events to relevant listeners.
#define POST_DELAYED_EVENT(EVENT_POINTER, DELAY)					\
	EventManager::getInstance()->postDelayedEvent(EVENT_POINTER, DELAY);

// Gets the internal pointer holding the queued events.
#define GET_POINTER_TO_QUEUED_EVENTS(EVENT_TYPE)					\
	EventManager::getInstance()->getPointerToQueuedEvents(EVENT_TYPE);

// Subscribes a IObserver to events of EventType.
#define SUBSCRIBE_TO_EVENT(SUBSCRIBER, EVENT_TYPE)					\
	EventManager::getInstance()->addObserver(SUBSCRIBER, EVENT_TYPE);

// Unsubscribes a IObserver to events of EventType.
#define UNSUBSCRIBE_TO_EVENT(SUBSCRIBER, EVENT_TYPE)				\
	EventManager::getInstance()->removeObserver(SUBSCRIBER, EVENT_TYPE);

// Unsubscribes a IObserver to all events.
#define UNSUBSCRIBE_TO_EVENTS(SUBSCRIBER)							\
	EventManager::getInstance()->removeObserver(SUBSCRIBER);

#define SUBSCRIBE_TO_EVENT(SUBSCRIBER,EVENT_TYPE)					\
	EventManager::getInstance()->addObserver(SUBSCRIBER, EVENT_TYPE);

// Fetches a owners of a specific Attribute from AttributeManager
#define SHOW_MESSAGEBOX(MESSAGE)									\
{																	\
	Event_ShowMessageBox e(MESSAGE);								\
	EventManager::getInstance()->sendEvent(&e);						\
}

// TODO
#define ATTRIBUTE_MANAGER											\
	AttributeManager::instance()

// Fetches a vector<AttributeType>* of a specific Attribute
// from AttributeManager.
#define GET_ATTRIBUTES(ATTRIBUTE_POINTER)							\
	&(AttributeManager::instance())->ATTRIBUTE_POINTER.attributes;


// Fetches a owners of a specific Attribute from AttributeManager
#define GET_ENTITIES(ENTITY_POINTER)								\
{																	\
	Event_GetEntities e;											\
	EventManager::getInstance()->sendEvent(&e);						\
	ENTITY_POINTER = e.entities;									\
}

// Fetches a owners of a specific Attribute from AttributeManager
#define GET_STATE()													\
	EventManager::getInstance()->state_TemporaryVariableUsedAsSubstituteForStateMachine								

// Defines DEBUGPRINT to an empty function in Release configuration
#ifndef XKILL_DEBUG
#define BULLETPHYSICSDEBUGDRAW false //render Bullet Physics debug lines true/false
#define DEBUGPRINT(DATA_STREAM)
#endif
// Defines DEBUGPRINT to a cout function in Debug configuration
#ifdef XKILL_DEBUG
#define BULLETPHYSICSDEBUGDRAW true //render Bullet Physics debug lines true/false
#include <sstream>
#define DEBUGPRINT(DATA_STREAM)									\
	{															\
	std::ostringstream oss;										\
	oss << DATA_STREAM << std::endl;							\
	printf(oss.str().c_str());									\
	}
#endif

// END OF EVIL