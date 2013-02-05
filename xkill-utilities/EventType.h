#pragma once

#include <vector>
#include "dllUtilities.h"
#include "Math.h"
#include "LightDesc.h"
#include "XKILL_Enums.h"
#include "AttributePointer.h"
#include "AttributeType.h"
#include "MeshDesc.h"
#include "MeshVertices.h"

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


// XKILL_Enums over each Event Type
/*
Used by EventManager to build each of its queues.

NOTE: DOXYGEN can not detect XKILL_Enums combined with DLL
for some reason
*/

enum DLL_U EventType
{
	// Inform events
	EVENT_QUIT_TO_DESKTOP,

	EVENT_PLAYERDEATH,
	EVENT_END_DEATHMATCH,
	EVENT_GAME_OVER,
	EVENT_START_DEATHMATCH,
	EVENT_CREATE_PROJECTILE,
	EVENT_CREATE_PLAYERSPAWNPOINT,
	EVENT_CREATE_PICKUPABLESSPAWNPOINT,
	EVENT_CREATE_PICKUPABLE,
	EVENT_CREATE_EXPLOSIONSPHERE,
	EVENT_CREATE_WORLD,
	EVENT_CREATE_LIGHT,
	EVENT_CREATE_ENTITY,
	EVENT_REMOVE_ENTITY,
	EVENT_STATE_CHANGED,
	EVENT_CREATE_INPUTDEVICE,

	EVENT_TRANSFEREVENTSTOGAME,

	EVENT_CHANGE_GAMESTATE,
	EVENT_GAMERESET,
	EVENT_UPDATE,
	EVENT_MOUSE_MOVE,
	EVENT_KEY_PRESS,
	EVENT_MOUSE_PRESS,
	EVENT_WINDOW_RESIZE,
	EVENT_INPUT_DEVICE_SEARCH,
	EVENT_PLAYSOUND,
	EVENT_UPDATESOUNDSETTINGS,
	EVENT_RUMBLE,
	EVENT_PHYSICS_ATTRIBUTES_COLLIDING,
	EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES,
	EVENT_SPLITSCREEN_CHANGED,
	EVENT_MODIFY_PHYSICS_OBJECT,

	EVENT_ATTRIBUTE_UPDATED,
	EVENT_SYNCSTATECOMMAND,

	// Get events
	EVENT_GET_ATTRIBUTE,
	EVENT_GET_ENTITIES,
	EVENT_GET_WINDOW_RESOLUTION,
	EVENT_GET_WINDOW_HANDLE,

	// Utilities
	EVENT_CREATE_MESH,
	EVENT_CREATE_LIGHT_D3D,
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

	enum sounds
	{
		SOUND_WALK,
		SOUND_HIT,
		SOUND_RESPAWN,
		SOUND_FIRE,
		SOUND_DEATH,
		SOUND_MUSIC,

		SOUND_LAST
	};
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

	int attributeEnum;			//!< An enums stored as an Int since we can't forward declare XKILL_Enums.
	void* hostVector;			//!< Void pointer to a vector holding Attributes.
								//!< Requires manual casting.
	std::vector<int>* owners;	//!< A std::vector<int> of owners corresponding to each
								//!< attribute.
};

/// Returns access to \ref ATTRIBUTES.
/**
\ingroup events
*/
class DLL_U Event_AttributeUpdated : public Event
{
public:
	Event_AttributeUpdated(int index, int attributeEnum) : Event(EVENT_ATTRIBUTE_UPDATED)
	{
		this->index = index;
		this->attributeEnum = attributeEnum;
		isCreated = false;
		isDeleted = false;
	}

	int attributeEnum;
	int index;
	bool isCreated;
	bool isDeleted;
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
	int entityIdOfCreator;
	XKILL_Enums::AmmunitionType ammunitionType;
	XKILL_Enums::FiringModeType firingMode;
	int damage;

	Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingMode, int damage);
};

class DLL_U Event_CreateMesh : public Event
{
public:
	unsigned int	id;
	MeshDesc		mesh;
	bool			dynamic;
	std::string		fileName;
	VertexType		vertexType;

	Event_CreateMesh(
		unsigned int	id, 
		MeshDesc		mesh, 
		bool			dynamic, 
		std::string		fileName, 
		VertexType		vertexType);
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
	bool isPressed;

	Event_KeyPress(int keyEnum, bool isPressed);
};

/// Alerts InputComponent about mouse press
/**
\ingroup events
*/
class DLL_U Event_MousePress : public Event
{
public:
	int keyEnum;
	bool isPressed;

	Event_MousePress(int keyEnum, bool isPressed);
};

class DLL_U Event_PlayerDeath : public Event
{
public:
	int playerIndex; //Index of the player that died

	Event_PlayerDeath(int playerIndex) : Event(EVENT_PLAYERDEATH)
	{
		this->playerIndex = playerIndex;
	}
};

class DLL_U Event_CreatePlayerSpawnPoint : public Event
{
public:
	Event_CreatePlayerSpawnPoint(Float3 spawnPointPosition, float spawnAreaRadius);

	Float3 spawnPointPosition;
	float spawnAreaRadius;
};

class DLL_U Event_CreatePickupablesSpawnPoint : public Event
{
public:
	Event_CreatePickupablesSpawnPoint(Float3 spawnPointPosition, XKILL_Enums::PickupableType pickupableType);

	Float3 spawnPointPosition;
	XKILL_Enums::PickupableType pickupableType;
};

class DLL_U Event_CreatePickupable : public Event
{
public:
	Event_CreatePickupable(Float3 position, XKILL_Enums::PickupableType pickupableType, AttributePtr<Attribute_PickupablesSpawnPoint> creatorPickupablesSpawnPoint, int amount);

	Float3 position;
	XKILL_Enums::PickupableType pickupableType;
	AttributePtr<Attribute_PickupablesSpawnPoint>  creatorPickupablesSpawnPoint;
	int amount;
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
class FiniteStateMachine;
/// Informs the engine about a state change
/**
Only for informative purposes, no intended to be used for changing state.
\sa SyncStateCommand
\ingroup events
*/
class DLL_U Event_StateChanged : public Event
{
public:
	Event_StateChanged(StateType newState, FiniteStateMachine* sender);

	FiniteStateMachine* sender;
	StateType newState;
};

/**
Event used for syncing state to state machines that have the same type
and current state as the sender but are different instances.
\ingroup events
*/
class DLL_U Event_SyncStateCommand : public Event
{
public:
	Event_SyncStateCommand(
		FiniteStateMachine* sender, 
		StateType fromState, 
		StateType toState, 
		bool isReplacementState);

	FiniteStateMachine* sender; 
	StateType fromState;
	StateType toState;
	bool isReplacementState;
};


class DLL_U Event_CreateExplosionSphere : public Event
{
public:
	Event_CreateExplosionSphere(Float3 position, float damage, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingModeType);

	Float3 position;
	float damage;
	int entityIdOfCreator;
	XKILL_Enums::AmmunitionType ammunitionType;
	XKILL_Enums::FiringModeType firingModeType;
};

class DLL_U Event_CreateWorld : public Event
{
public:
	Event_CreateWorld(Float3 position, Float4 rotation, unsigned int meshID);

	Float3 position;
	Float4 rotation;
	unsigned int meshID;
};

class DLL_U Event_CreateLight : public Event
{
public:
	Event_CreateLight(Float3 position, Float3 direction, Float3 ambient, Float3 diffuse, Float3 specular,
					  Float3 attenuation, float range, float spotPow, unsigned int type);

	Float3 position;
	Float3 direction;
	Float3 ambient;
	Float3 diffuse;
	Float3 specular;
	Float3 attenuation;
	float range;
	float spotPow;
	unsigned int type;
};

class DLL_U Event_TransferEventsToGame : public Event
{
public:
	Event_TransferEventsToGame(std::vector<Event*> events);
	std::vector<Event*> events;
};

enum DLL_U EntityType
{
	WORLD,
	PLAYER,
	PROJECTILE,
	INPUTDEVICES,
	RENDERABLE
};

class DLL_U Event_CreateEntity : public Event
{
public:
	Event_CreateEntity(EntityType entityType);

	EntityType entityType;
};

class InputObjectArray;
class InputDevice;
class DLL_U Event_CreateInputDevice : public Event
{
public:
	Event_CreateInputDevice(InputDevice* inputDevice, InputObjectArray* inputObjectArray);

	InputDevice* device;
	InputObjectArray* objectArray;
};

/*
class DLL_U Event_DrawDebugLine : public Event
{
public:
	Event_DrawDebugLine(Float3 vertex1, Float3 vertex2);

	Float3 vertex1;
	Float3 vertex2;
};
*/

#include "MeshVertices.h"
class DLL_U Event_DrawBulletPhysicsDebugLines : public Event
{
public:
	Event_DrawBulletPhysicsDebugLines(std::vector<VertexPosColor>* debugLineVertices);

	std::vector<VertexPosColor>* debugLineVertices;
};

class DLL_U Event_ModifyPhysicsObject : public Event
{
public:
	Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData modifyWhatDataInPhysicsObjectData, void* data, int physicsAttributeIndex);

	XKILL_Enums::ModifyPhysicsObjectData modifyWhatDataInPhysicsObjectData;
	void* data;
	int physicsAttributeIndex;
};
