#pragma once

#include <vector>
#include "dllUtilities.h"
#include "Math.h"

//
// Events info
//

/// Used to communicate with \ref COMPONENTS and other IObserver throught the use of EventManager.
/** 
Memory deletion should be made by the funciton sending the Event.

The following usage i prefered:
\code
Event_A event;
EventManager::getInstance()->sendEvent(&event);
\endcode

\defgroup EVENTS Events
\ingroup UTILITIES
\sa EventManager
*/


// Enums over each Event Type
/*
Used by EventManager to build each of its queues.

NOTE: DOXYGEN can not detect Enums combined with DLL
for some reason
*/

enum DLL_U EventType
{
	// Inform events
	EVENT_A,
	EVENT_B,
	EVENT_PLAYSOUND,
	EVENT_RUMBLE,
	EVENT_CREATEPROJECTILE,

	EVENT_MOUSEMOVE,
	EVENT_KEYPRESS,
	EVENT_KEYRELEASE,

	// Get events
	EVENT_GET_ATTRIBUTE,
	EVENT_GET_WINDOW_RESOLUTION,
	EVENT_GET_WINDOW_HANDLE,

	// Utilities
	EVENT_SHOW_MESSAGEBOX,

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
\ingroup EVENTS
*/
class DLL_U Event_MouseMove : public Event
{
public:
	Event_MouseMove(int dx, int dy) : Event(EVENT_MOUSEMOVE)
	{
		this->dx = dx;
		this->dy = dy;
	}

	int dx;
	int dy;
};

/**
\ingroup events
*/
class DLL_U Event_PlaySound : public Event
{
public:
	Event_PlaySound(int soundId) : Event(EVENT_PLAYSOUND)
	{
		this->soundId = soundId;
	}

	int soundId;
};

//! 
class DLL_U Event_Rumble : public Event
{
public:
	Event_Rumble(unsigned int deviceNr, bool runRumble, float duration, float leftScale, float rightScale) : 
		Event(EVENT_RUMBLE){this->deviceNr = deviceNr; this->runRumble = runRumble; this->duration = duration; this->leftScale = leftScale; this->rightScale = rightScale;}
	unsigned int deviceNr;
	bool runRumble;
	float duration;
	float leftScale;
	float rightScale;
};

/// Returns acces to \ref ATTRIBUTES.
/**
\ingroup events
*/
class DLL_U Event_getAttribute : public Event
{
public:
	Event_getAttribute(int attributeEnum) : Event(EVENT_GET_ATTRIBUTE)
	{
		this->attributeEnum = attributeEnum;
		
		hostVector = 0;
		owners = 0;
	}

	int attributeEnum;			//!< An enums stored as an Int since we can't forward declare Enums.
	void* hostVector;			//!< Void pointer to a vector holding Attributes.
								//!< Requires manual casting.
	std::vector<int>* owners;	//!< A std::vector<int> of owners correspoinding to each
								//!< attribute.
};

/// Returns window resolution.
/**
\ingroup events
*/
class DLL_U Event_getWindowResolution : public Event
{
public:
	int width;		
	int height;		

	Event_getWindowResolution() : Event(EVENT_GET_WINDOW_RESOLUTION)
	{
		width = 320;
		height = 240;
	}

	float getAspectRatio()
	{
		return (float)width/(float)height;
	}
};

/// Displays a messagebox with the message specified
/**
\ingroup events
*/
class DLL_U Event_showMessageBox : public Event
{
public:
	std::string message;

	Event_showMessageBox(std::string message) : Event(EVENT_SHOW_MESSAGEBOX)
	{
		this->message = message;
	}
};

class DLL_U Event_createProjectile : public Event
{
public:
	Float3 position;
	Float4 direction;

	Event_createProjectile(Float3 position, Float4 direction) : Event(EVENT_CREATEPROJECTILE)
	{
		this->position = position;
		this->direction = direction;
	}
};

class DLL_U Event_KeyPress : public Event
{
public:
	int keyEnum;

	Event_KeyPress(int keyEnum) : Event(EVENT_KEYPRESS)
	{
		this->keyEnum = keyEnum;
	}
};

class DLL_U Event_KeyRelease : public Event
{
public:
	int keyEnum;

	Event_KeyRelease(int keyEnum) : Event(EVENT_KEYRELEASE)
	{
		this->keyEnum = keyEnum;
	}
};