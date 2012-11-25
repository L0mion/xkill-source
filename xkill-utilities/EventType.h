#pragma once

#include "dllUtilities.h"

//
// Events info
//

/// Used to communicate with \ref components and other IObserver throught the use of EventManager.
/** 
Memory deletion should be made by the funciton sending the Event.

The following usage i prefered:
Event_A event;
EventManager::getInstance()->sendEvent(&event);

\defgroup events Events
\ingroup utilities
\sa EventManager
*/


/// Enums over each Event Type
/*
Used by EventManager to build each of its queues.

NOTE: DOXYGEN can not detect Enums combined with DLL
for some reason
*/

enum DLL EventType
{
	EVENT_A,
	EVENT_B,
	EVENT_C,
	EVENT_D,
	EVENT_E,
	EVENT_F,

	// this is needed, don't touch!
	EVENT_LAST 
};


/// Base class
/**
\ingroup events
*/

class DLL Event
{
private:
	EventType type;
public:
	Event(EventType type) 
	{ 
		this->type = type; 
	}

	EventType getType() 
	{ 
		return type; 
	}
};



////////////////////////////////////////////
// Events
////////////////////////////////////////////

/**
\ingroup events
*/
class DLL Event_A : public Event
{
public:
	Event_A() : Event(EVENT_A){}

	int a;
};

/**
\ingroup events
*/
class DLL Event_B : public Event
{
public:
	Event_B() : Event(EVENT_B){}

	int b;
};