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
\ingroup UTILITIE
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
	EVENT_PLAYSOUND,
	EVENT_RUMBLE,
	EVENT_CREATE_PROJECTILE,
	EVENT_PHYSICS_ATTRIBUTES_COLLIDING,
	EVENT_REMOVE_ENTITY,
	EVENT_PLAYERDEATH,
	EVENT_CREATE_SPAWNPOINT,
	EVENT_END_DEATHMATCH,
	EVENT_START_DEATHMATCH,
	EVENT_CHANGE_GAMESTATE,
	EVENT_CREATE_EXPLOSIONSPHERE,

	EVENT_CREATE_ENTITY,
	EVENT_GAMERESET,
	EVENT_UPDATE,
	EVENT_MOUSE_MOVE,
	EVENT_KEY_PRESS,
	EVENT_KEY_RELEASE,
	EVENT_WINDOW_RESIZE,

	EVENT_DO_CULLING,

	// Get events
	EVENT_GET_ATTRIBUTE,
	EVENT_GET_ENTITIES,
	EVENT_GET_WINDOW_RESOLUTION,
	EVENT_GET_WINDOW_HANDLE,

	// Utilities
	EVENT_CREATE_MESH,
	EVENT_LOAD_TEXTURES,
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
	Event(EventType type);

	EventType getType();
};



////////////////////////////////////////////
// Events
////////////////////////////////////////////

/**
\ingroup EVENTS
*/
class DLL_U Event_MouseMove : public Event
{
public:
	Event_MouseMove(int dx, int dy);

	int dx;
	int dy;
};

/**
If muteSound is true then all sounds will be muted

\ingroup events
*/
class DLL_U Event_PlaySound : public Event
{
public:
	Event_PlaySound(int soundId, bool muteSound = false);

	int soundId;
	bool muteSound;
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
	Event_Rumble(unsigned int deviceNr,
				 bool runRumble,
				 float duration,
				 float leftScale,
				 float rightScale);
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
	Event_GetAttribute(int attributeEnum);

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
	Event_GetEntities();

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

	Event_GetWindowResolution();

	float getAspectRatio();
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

	Event_WindowResize(int width, int height);

	float getAspectRatio();
};



/// Displays a messagebox with the message specified
/**
\ingroup events
*/
class DLL_U Event_ShowMessageBox : public Event
{
public:
	std::string message;

	Event_ShowMessageBox(std::string message);
};

/// Tells EntityManager to create a projectile entity
/**
\ingroup events
*/
class DLL_U Event_CreateProjectile : public Event
{
public:
	Float3 position;
	Float3 velocity;
	Float4 rotation;
	int damage;
	int entityIdOfCreator;
	bool explodeOnImpact;

	Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, int damage, int entityIdOfCreator, bool explodeOfImpact);
};

class MeshModel;
class DLL_U Event_CreateMesh : public Event
{
public:
	unsigned int	id;
	MeshModel*		mesh;
	bool			dynamic;

	Event_CreateMesh(unsigned int id, MeshModel* mesh, bool dynamic);
};

class TexDesc;
//! Event sending information of which textures to load to renderer. Renderer then cleans the memory allocated manually.
class DLL_U Event_LoadTextures : public Event
{
public:
	TexDesc* texDesc_;

	Event_LoadTextures(TexDesc* texDesc) : Event(EVENT_LOAD_TEXTURES)
	{
		texDesc_ = texDesc;
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

	Event_PhysicsAttributesColliding(int attribute1_index,
									 int attribute2_index);
};

/// Alerts EntityManager to remove an entity with specified id
/**
\ingroup events
*/
class DLL_U Event_RemoveEntity : public Event
{
public:
	int entityId;

	Event_RemoveEntity(int entityId);
};

/// Alerts InputComponent about key press
/**
\ingroup events
*/
class DLL_U Event_KeyPress : public Event
{
public:
	int keyEnum;

	Event_KeyPress(int keyEnum);
};

/// Alerts InputComponent about key release
/**
\ingroup events
*/
class DLL_U Event_KeyRelease : public Event
{
public:
	int keyEnum;

	Event_KeyRelease(int keyEnum);
};

class DLL_U Event_PlayerDeath : public Event
{
public:
	Event_PlayerDeath() : Event(EVENT_PLAYERDEATH)
	{
	}
};

class DLL_U Event_CreateSpawnPoint : public Event
{
public:
	Event_CreateSpawnPoint(Float3 spawnPointPosition, float spawnAreaRadius);

	Float3 spawnPointPosition;
	float spawnAreaRadius;
};


class DLL_U Event_DoCulling : public Event
{
public:
	Event_DoCulling();
};


class DLL_U Event_StartDeathmatch : public Event
{
public:
	Event_StartDeathmatch(int num_players);
	int num_players;
};

class DLL_U Event_EndDeathmatch : public Event
{
public:
	Event_EndDeathmatch();
};

enum StateType;
class DLL_U Event_ChangeGameState : public Event
{
public:
	Event_ChangeGameState(StateType newState);

	StateType newState;
};

class DLL_U Event_CreateExplosionSphere : public Event
{
public:
	Event_CreateExplosionSphere(Float3 position, float radius, int damage, int entityIdOfCreator);

	Float3 position;
	float radius;
	int damage;
	int entityIdOfCreator;
};

enum DLL_U EntityType
{
	WORLD,
	PLAYER,
	PROJECTILE
};

class DLL_U Event_CreateEntity : public Event
{
public:
	Event_CreateEntity(EntityType entityType);

	EntityType entityType;
};

