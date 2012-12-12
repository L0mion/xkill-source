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
	EVENT_CREATE_PROJECTILE,
	EVENT_ENTITIES_COLLIDING,
	EVENT_REMOVE_ENTITY,
	EVENT_PLAYERKILLED,

	EVENT_UPDATE,
	EVENT_MOUSE_MOVE,
	EVENT_KEY_PRESS,
	EVENT_KEY_RELEASE,
	EVENT_WINDOW_RESIZE,

	// Get events
	EVENT_GET_ATTRIBUTE,
	EVENT_GET_ENTITIES,
	EVENT_GET_WINDOW_RESOLUTION,
	EVENT_GET_WINDOW_HANDLE,

	// Utilities
	EVENT_CREATE_MESH,
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
	Event_MouseMove(int dx, int dy) : Event(EVENT_MOUSE_MOVE)
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

//! Will trigger rumble in devices[deviceNr]
/*!
\param deviceNr The device to be triggered
\param runRumble If rumble should run or not
\param duration Duration the effect will be active (currently not handled)
\param leftScale Power of the left motor
\param rightScale Power of the right motor
\ingroup events
*/
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

/// Returns access to \ref ATTRIBUTES.
/**
\ingroup events
*/
class DLL_U Event_GetAttribute : public Event
{
public:
	Event_GetAttribute(int attributeEnum) : Event(EVENT_GET_ATTRIBUTE)
	{
		this->attributeEnum = attributeEnum;
		
		hostVector = 0;
		owners = 0;
	}

	int attributeEnum;			//!< An enums stored as an Int since we can't forward declare Enums.
	void* hostVector;			//!< Void pointer to a vector holding Attributes.
								//!< Requires manual casting.
	std::vector<int>* owners;	//!< A std::vector<int> of owners corresponding to each
								//!< attribute.
};

/// Returns access to a vector of Entity from EntityManager.
/**
\ingroup events
*/
class Entity;
class DLL_U Event_GetEntities : public Event
{
public:
	Event_GetEntities() : Event(EVENT_GET_ENTITIES)
	{
	}

	std::vector<Entity>* entities;
};

/// Returns window resolution.
/**
\ingroup events
*/
class DLL_U Event_GetWindowResolution : public Event
{
public:
	int width;		
	int height;		

	Event_GetWindowResolution() : Event(EVENT_GET_WINDOW_RESOLUTION)
	{
		width = 320;
		height = 240;
	}

	float getAspectRatio()
	{
		return (float)width/(float)height;
	}
};

/// Alerts about change in window resolution
/**
\ingroup events
*/
class DLL_U Event_WindowResize : public Event
{
public:
	int width;		
	int height;		

	Event_WindowResize(int width, int height) : Event(EVENT_WINDOW_RESIZE)
	{
		this->width = width;
		this->height = height;
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
class DLL_U Event_ShowMessageBox : public Event
{
public:
	std::string message;

	Event_ShowMessageBox(std::string message) : Event(EVENT_SHOW_MESSAGEBOX)
	{
		this->message = message;
	}
};

/// Tells EntityManager to create a projectile entity
/**
\ingroup events
*/
class DLL_U Event_CreateProjectile : public Event
{
public:
	int entityIdOfCreator;
	Float3 position;
	Float3 velocity;
	Float4 rotation;

	Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, int entityIdOfCreator) : Event(EVENT_CREATE_PROJECTILE)
	{
		this->entityIdOfCreator = entityIdOfCreator;
		this->position = position;
		this->velocity = velocity;
		this->rotation = rotation;
	}
};

class MeshModel;
class DLL_U Event_CreateMesh : public Event
{
public:
	MeshModel*	mesh;
	bool		dynamic;

	Event_CreateMesh(MeshModel* mesh, bool dynamic) : Event(EVENT_CREATE_MESH)
	{
		this->mesh		= mesh;
		this->dynamic	= dynamic;
	}
};

/// Used in GameComponent
/**
\ingroup events
*/
class DLL_U Event_PhysicsAttributesColliding : public Event
{
public:
	int attribute1_index;
	int attribute2_index;

	Event_PhysicsAttributesColliding(int attribute1_index, int attribute2_index) : Event(EVENT_ENTITIES_COLLIDING)
	{
		this->attribute1_index = attribute1_index;
		this->attribute2_index = attribute2_index;
	}
};

/// Alerts EntityManager to remove an entity with specified id
/**
\ingroup events
*/
class DLL_U Event_RemoveEntity : public Event
{
public:
	int entityId;

	Event_RemoveEntity(int entityId) : Event(EVENT_REMOVE_ENTITY)
	{
		this->entityId = entityId;
	}
};

/// Alerts InputComponent about key press
/**
\ingroup events
*/
class DLL_U Event_KeyPress : public Event
{
public:
	int keyEnum;

	Event_KeyPress(int keyEnum) : Event(EVENT_KEY_PRESS)
	{
		this->keyEnum = keyEnum;
	}
};

/// Alerts InputComponent about key release
/**
\ingroup events
*/
class DLL_U Event_KeyRelease : public Event
{
public:
	int keyEnum;

	Event_KeyRelease(int keyEnum) : Event(EVENT_KEY_RELEASE)
	{
		this->keyEnum = keyEnum;
	}
};
