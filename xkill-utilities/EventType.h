#pragma once

#include "dllUtilities.h"

//
// Events info
//

/// Used to communicate with \ref COMPONENTS and other IObserver throught the use of EventManager.
/** 
Memory deletion should be made by the funciton sending the Event.

The following usage i prefered:
Event_A event;
EventManager::getInstance()->sendEvent(&event);

\defgroup EVENTS Events
\ingroup UTILITIES
\sa EventManager
*/


/// Enums over each Event Type
/*
Used by EventManager to build each of its queues.

NOTE: DOXYGEN can not detect Enums combined with DLL
for some reason
*/

enum DLL_U EventType
{
	EVENT_A,
	EVENT_B,
	EVENT_C,
	EVENT_D,
	EVENT_E,
	EVENT_F,
	PLAYSOUND,
	RUMBLE,

	// this is needed, don't touch!
	EVENT_LAST 
};


/// Base class
/**
\ingroup EVENTS
*/

class DLL_U Event
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
\ingroup EVENTS
*/
class DLL_U Event_A : public Event
{
public:
	Event_A() : Event(EVENT_A){}

	int a;
};

/**
\ingroup EVENTS
*/
class DLL_U Event_B : public Event
{
public:
	Event_B() : Event(EVENT_B){}

	int b;
};

/**
\ingroup events
*/
class DLL_U Event_PlaySound : public Event
{
public:
	Event_PlaySound(int soundId) : Event(PLAYSOUND){this->soundId=soundId;}
	int soundId;
};

//! 
class DLL_U Event_Rumble : public Event
{
public:
	Event_Rumble(unsigned int deviceNr, bool runRumble, float duration, float leftScale, float rightScale) : 
		Event(RUMBLE){this->deviceNr = deviceNr; this->runRumble = runRumble; this->duration = duration; this->leftScale = leftScale; this->rightScale = rightScale;}
	unsigned int deviceNr;
	bool runRumble;
	float duration;
	float leftScale;
	float rightScale;
};