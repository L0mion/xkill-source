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
#include "SkinnedData.h"

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
	// Game states
	EVENT_QUIT_TO_DESKTOP,
	EVENT_ENABLE_MENU,
	EVENT_ENABLE_HUD,
	EVENT_SHOW_FULLSCREEN,
	EVENT_TOGGLE_EDITOR,

	// Game-loop
	EVENT_STARTGAME,
	EVENT_ENDGAME,
	EVENT_UPDATE,
	EVENT_GAMEOVER,
	EVENT_START_DEATHMATCH,
	EVENT_END_DEATHMATCH,
	EVENT_GAMERESET,

	// System
	EVENT_TOGGLE_DEBUG_MESSAGES,
	EVENT_POST_DEBUG_MESSAGE,
	EVENT_ATTRIBUTE_UPDATED,
	EVENT_UPDATESOUNDSETTINGS,
	EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES,
	EVENT_MODIFY_PHYSICS_OBJECT,
	EVENT_RELOAD_PHYSICS_ATTRIBUTE_DATA_INTO_BULLET_PHYSICS,

	// Input
	EVENT_KEY_PRESS,
	EVENT_MOUSE_PRESS,
	EVENT_MOUSE_MOVE,
	EVENT_MOUSE_WHEEL,
	EVENT_SET_MOUSELOCK,
	EVENT_WINDOW_RESIZE,
	EVENT_WINDOW_MOVE,
	EVENT_WINDOW_FOCUS_CHANGED,
	EVENT_SPLITSCREEN_CHANGED,
	EVENT_RUMBLE,
	EVENT_PLAYSOUND,
	EVENT_STOPSOUND,
	EVENT_FOCUS_MAINWINDOW,
	EVENT_INPUT_DEVICE_SEARCH,

	EVENT_ANIMATION_LOADED,

	// Get events
	EVENT_GET_ENTITIES,
	EVENT_GET_WINDOW_RESOLUTION,
	EVENT_GET_WINDOW_HANDLE,

	// Gameplay events
	EVENT_PLAYERDEATH,
	EVENT_PLAYER_TARGET_HIT,
	EVENT_PLAYER_TAKING_DAMAGE,
	EVENT_PHYSICS_ATTRIBUTES_COLLIDING,
	EVENT_SYNC_STATE_COMMAND,
	EVENT_CLOSEST_HIT_RAY_CAST,
	EVENT_ALL_HITS_RAY_CAST,
	EVENT_PLAYER_EXECUTING,
	EVENT_PLAYER_DONE_EXECUTING,
	EVENT_GET_FILE_LIST,
	EVENT_POST_HUD_MESSAGE,
	EVENT_NULL_PROCESS_STARTED_EXECUTING,
	EVENT_NULL_PROCESS_STOPPED_EXECUTING,
	EVENT_SPAWN_PLAYER,

	// Creation/Destruction
	EVENT_TRANSFER_EVENTS_TO_GAME,
	EVENT_CREATE_PROJECTILE,
	EVENT_CREATE_PLAYER_SPAWNPOINT,
	EVENT_CREATE_PICKUPABLES_SPAWNPOINT,
	EVENT_CREATE_PICKUPABLE,
	EVENT_CREATE_EXPLOSIONSPHERE,
	EVENT_CREATE_WORLD,
	EVENT_CREATE_LIGHT,
	EVENT_CREATE_CORPSE,
	EVENT_CREATE_ENTITY,
	EVENT_REMOVE_ENTITY,
	EVENT_STATE_CHANGED,
	EVENT_CREATE_INPUTDEVICE,
	EVENT_HACK_ACTIVATED,
	EVENT_CHANGE_GAMESTATE,

	// Utilities
	EVENT_CREATE_MESH,
	EVENT_CREATE_LIGHT_D3D,
	EVENT_LOAD_TEXTURES,
	EVENT_LOAD_LEVEL,
	EVENT_LOAD_LEVEL_BULLET,
	EVENT_UNLOAD_LEVEL,
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

class DLL_U Event_SetFullscreen : public Event
{
public:
	Event_SetFullscreen(bool on);

	bool on;
};

class DLL_U Event_PostDebugMessage : public Event
{
public:
	Event_PostDebugMessage(int index, std::string message) : Event(EVENT_POST_DEBUG_MESSAGE)
	{
		this->index = index;
		this->message = message;
	}

	int index;
	std::string message;
};

class DLL_U Event_PlayerTargetHit : public Event
{
public:
	Event_PlayerTargetHit(AttributePtr<Attribute_Player> ptr_playern) : Event(EVENT_PLAYER_TARGET_HIT)
	{
		this->ptr_player = ptr_playern;
	}

	AttributePtr<Attribute_Player> ptr_player; // The player that hit another target
};

class DLL_U Event_PlayerTakingDamage : public Event
{
public:
	Event_PlayerTakingDamage(AttributePtr<Attribute_Player> ptr_playern) : Event(EVENT_PLAYER_TAKING_DAMAGE)
	{
		this->ptr_player = ptr_playern;
	}

	AttributePtr<Attribute_Player> ptr_player; // The player that hit another target
};

class DLL_U Event_PostHudMessage : public Event
{
public:
	enum Receiver
	{
		RECEIVER_ONLY_SUBJECT,
		RECEIVER_ALL_BUT_SUBJECT,
		RECEIVER_ALL,
	};
	enum Style
	{
		STYLE_NORMAL,
		STYLE_SUBTILE,
		STYLE_WARNING
	};

	Event_PostHudMessage(std::string message, AttributePtr<Attribute_Player> ptr_subject_player = AttributePtr<Attribute_Player>() );
	void setStyle(Style style);
	void setHtmlMessage(std::string prefex, std::string subject = "", std::string suffix = "", std::string description = "");
	void setColor(Float3 color);

	Receiver receiver;
	std::string message;
	AttributePtr<Attribute_Player> ptr_subject_player;
	std::string styleSheet;
	Float4 color;
};

/**
\ingroup events
*/
class DLL_U Event_PlaySound : public Event
{
public:
	/**
	\param ownerPlayerId Set to '-1' if no owner
	*/
	Event_PlaySound(int soundId, int ownerEntityId = -1, Float3 position = Float3(), bool use3DAudio = false);

	int soundId;
	int ownerEntityId;
	Float3 position;
	bool use3DAudio;
};

/**
\ingroup events
*/
class DLL_U Event_StopSound : public Event
{
public:
	Event_StopSound(int soundId, int ownerEntityId = -1);

	int soundId;
	int ownerEntityId;
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

class DLL_U Event_EnableHud : public Event
{
public:
	Event_EnableHud(bool enableHud) : Event(EVENT_ENABLE_HUD)
	{
		this->enableHud = enableHud;
	}
	bool enableHud;
};

class DLL_U Event_EnableMenu : public Event
{
public:
	Event_EnableMenu(bool enableMenu) : Event(EVENT_ENABLE_MENU)
	{
		this->enableMenu = enableMenu;
	}
	bool enableMenu;
};

/// Returns access to \ref ATTRIBUTES.
/**
\ingroup events
*/
class DLL_U Event_AttributeUpdated : public Event
{
public:
	int attributeEnum;
	int index;
	bool isCreated;
	bool isDeleted;

	Event_AttributeUpdated(int index, int attributeEnum);
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

class DLL_U Event_WindowMove : public Event
{
public:
	Int2 pos;
	Int2 oldPos;

	Event_WindowMove(Int2 pos, Int2 oldPos);
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
	float damage;

	Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingMode, float damage);
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
	Event_LoadTextures(TexDesc* texDesc);
	
	TexDesc* texDesc_;
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
	bool shiftPressed;
	bool tabPressed;

	Event_KeyPress(int keyEnum, bool isPressed, bool shiftPressed, bool tabPressed);
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

class DLL_U Event_MouseWheel : public Event
{
public:
	int value;

	Event_MouseWheel(int value);
};

class DLL_U Event_SetMouseLock : public Event
{
public:
	bool isLock;

	Event_SetMouseLock(bool isLock);
};

class DLL_U Event_PlayerDeath : public Event
{
public:
	Event_PlayerDeath(int playerAttributeIndex);
	
	int playerAttributeIndex; //Attribute index of the player that died
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
	Event_CreateExplosionSphere(Float3 position, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingModeType);

	Float3 position;
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

class DLL_U Event_CreateCorpse : public Event
{
public:
	//! Create a corpse matching player
	Event_CreateCorpse(AttributePtr<Attribute_Player> ptr_player);
	
	AttributePtr<Attribute_Player> ptr_player;
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
	Entity* entity;
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
	Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData modifyWhatDataInPhysicsObjectData, void* data, AttributePtr<Attribute_Physics> ptr_physics);

	AttributePtr<Attribute_Physics> ptr_physics;
	XKILL_Enums::ModifyPhysicsObjectData modifyWhatDataInPhysicsObjectData;
	void* data;
};

class DLL_U Event_HackActivated : public Event
{
public:
	Event_HackActivated(float time, XKILL_Enums::HackType hackType, AttributePtr<Attribute_Player> player);

	float time;
	XKILL_Enums::HackType hackType;
	AttributePtr<Attribute_Player> player;
};

class DLL_U Event_ClosestHitRayCast : public Event
{
public:
	Event_ClosestHitRayCast(Float3 from, Float3 to, short collisionFilterMask);

	//Event input variables:
	Float3 from;										//!< Point from where the ray originates
	Float3 to;											//!< Point where the ray ends
	short collisionFilterMask;							//!< What types of physics objects (refer to XKILL_Enums::PhysicsAttributeType) the ray should collide with during its travel from "from" to "to"
	
	//Event output variables:
	int EntityIdOfOwnerToClosestPhysicsObjectHitByRay;	//!< Set to 0 if no entity was hit by the ray
	Float3 ClosestHitPoint;								//!< If the ray did not hit any physics object, "ClosestHitPoint" will be set to "to".
};

class DLL_U Event_AllHitsRayCast : public Event
{
public:
	Event_AllHitsRayCast(Float3 from, Float3 to, short collisionFilterMask);

	//Event input variables:
	Float3 from;										//!< Point from where the ray originates
	Float3 to;											//!< Point where the ray ends
	short collisionFilterMask;							//!< What types of physics objects (refer to XKILL_Enums::PhysicsAttributeType) the ray should collide with during its travel from "from" to "to"
	
	//Event output variables:
	std::vector<std::pair<Float3, int>> mapHitPointToEntityId; //!< Empty if the ray did not hit anything
};

class DLL_U Event_PlayerExecuting : public Event
{
public:
	Event_PlayerExecuting(int executingPlayerIndex);

	int executingPlayerIndex;
};	

class DLL_U Event_GetFileList : public Event
{
public:
	/**
	\param filepathAndExtension Should look like this: path\*.extension
	*/
	Event_GetFileList(std::string filepath, std::string extension); 

	std::string filepath;
	std::string extension;
	std::vector<std::string> filenames;
};

class DLL_U Event_LoadLevel : public Event
{
public:
	Event_LoadLevel(std::string levelName);

	std::string levelName;
};

class DLL_U Event_AnimationLoaded : public Event
{
public:
	Event_AnimationLoaded(unsigned int meshID, SkinnedData* skinnedData);

	unsigned int meshID;
	SkinnedData* skinnedData;
};

class DLL_U Event_ReloadPhysicsAttributeDataIntoBulletPhysics : public Event
{
public:
	Event_ReloadPhysicsAttributeDataIntoBulletPhysics(int physicsAttributeId);

	int physicsAttributeId;
};

class DLL_U Event_SpawnPlayer : public Event
{
public:
	Event_SpawnPlayer(int playerAttributeId);

	int playerAttributeId;
};
