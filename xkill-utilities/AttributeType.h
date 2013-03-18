#pragma once

#include <string>

#include "dllUtilities.h"
#include "AttributePointer.h"
#include "LightDesc.h"
#include "XKILL_Enums.h"
#include "MeshDesc.h"
#include "MeshVertices.h"
#include "Math.h"
#include "Timer.h"

struct DataItem;
class DataItemList;
/// Used inside \ref COMPONENTS for data processing.
/** 
Created by AttributeManager and stored in a AttributeStorage.

An attribute can be shared among multiple Components which allows
each Component to work with the same data independently of eachother
with minimal overhead.

The attributes listed is currently only placeholders and should
be modified to suit the need of each Component.

\defgroup ATTRIBUTES Attributes
\ingroup UTILITIES
*/


///////////////////////////////////////////
// Attributes
///////////////////////////////////////////

// Enums over each Attribute Type
/*
Just something that can be used if "casting" of
\ref ATTRIBTUES is needed.
NOTE: DOXYGEN can not detect XKILL_Enums combined with DLL
for some reason
*/


enum DLL_U AttributeType
{
	ATTRIBUTE_UNKOWN,

	ATTRIBUTE_SETTINGS,
	ATTRIBUTE_POSITION,
	ATTRIBUTE_SPATIAL,
	ATTRIBUTE_RENDER,
	ATTRIBUTE_PHYSICS,
	ATTRIBUTE_CAMERA,
	ATTRIBUTE_SPLITSCREEN,
	ATTRIBUTE_INPUT,
	ATTRIBUTE_INPUTDEVICE,
	ATTRIBUTE_SOUND,
	ATTRIBUTE_SOUNDSETTINGS,

	ATTRIBUTE_PLAYER,
	ATTRIBUTE_BOUNDING,
	ATTRIBUTE_MESH,
	ATTRIBUTE_PROJECTILE,
	
	ATTRIBUTE_LIGHT_DIRECTIONAL,
	ATTRIBUTE_LIGHT_POINT,
	ATTRIBUTE_LIGHT_SPOT,

	ATTRIBUTE_HEALTH,
	ATTRIBUTE_DAMAGE,
	ATTRIBUTE_PLAYERSPAWNPOINT,
	ATTRIBUTE_PICKUPABLESSPAWNPOINT,
	ATTRIBUTE_PICKUPABLE,
	ATTRIBUTE_WEAPONSTATS,
	ATTRIBUTE_EXPLOSIONSPHERE,
	ATTRIBUTE_RAY,

	ATTRIBUTE_ANIMATION,

	// Behaviors are attributes with more logic tied to them,
	// and be should independent of other attributes (should not have pointer to them)
	// not sure about this yet
	BEHAVIOR_OFFSET,

	// this is needed, don't touch!
	ATTRIBUTE_LAST
};

/// Attribute interface to facilitate grouping of attributes.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U IAttribute
{
	IAttribute();

	//! Called when deleting attributes (refer to AttributeStorage::deleteAttribute(int index))
	virtual void clean(){};
	virtual ~IAttribute();

	virtual DataItemList* getDataList();
	virtual void saveTo(DataItemList* list);
	virtual AttributeType getType(){return ATTRIBUTE_UNKOWN;}
	virtual std::string getName(){return "UNKOWN";}
};


/// Stores the position of an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Position : public IAttribute
{
	Attribute_Position();
	~Attribute_Position();

	Float3 position;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_POSITION;}
	std::string getName(){return "Position";}
};

/// Stores detailed Spatial information about an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Spatial : public IAttribute
{
	Attribute_Spatial();
	~Attribute_Spatial();

	AttributePtr<Attribute_Position> ptr_position;

	Float4 rotation;
	Float3 scale;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_SPATIAL;}
	std::string getName(){return "Spatial";}
};

struct DLL_U Behavior_Offset : public IAttribute
{
private:
	

public:
	AttributePtr<Attribute_Spatial> ptr_parent_spatial_position;
	AttributePtr<Attribute_Spatial> ptr_parent_spatial_rotation;
	AttributePtr<Attribute_Spatial> ptr_spatial;

	Float3 offset_position;
	Float4 offset_rotation;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);

	void updateOffset();

	AttributeType getType(){return BEHAVIOR_OFFSET;}
	std::string getName(){return "Offset";}
};

/// Stores the points for both a bounding box and a convex mesh
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Bounding : public IAttribute
{
	float BoxPoints[8*3];
	float ConvexPoints[42*3];
	DataItemList* getDataList();
	AttributeType getType(){return ATTRIBUTE_BOUNDING;}
	std::string getName(){return "Bounding";}
};


struct DLL_U Attribute_Animation : public IAttribute
{
	Attribute_Animation();
	~Attribute_Animation();

	std::string activeAnimation;
	float time;

	AttributeType getType(){return ATTRIBUTE_ANIMATION;}
	std::string getName(){return "Animation";}
};


/// Stores everything RenderComponent needs to know about an entity
/** 
A good approach for the RenderComponent would be to step through all 
RenderAttribute and construct multiple queues consisting of objects
that should be transparent or not, tessellated or not, use the same
meshID and textureID, and the Render each Queue in a orderly fashion 
through the use of Instancing.

\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Render : public IAttribute
{
	Attribute_Render();
	~Attribute_Render();

	AttributePtr<Attribute_Spatial> ptr_spatial;
	AttributePtr<Attribute_Bounding> ptr_bounding;
	AttributePtr<Attribute_Animation> ptr_animation;
	
	int meshID;
	int textureID;

	//bool transparent;
	//bool tessellation;

	BoolField culling;
	bool cull;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_RENDER;}
	std::string getName(){return "Render";}
};

/// Stores everything PhysicsComponent needs to know about an entity
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Physics : public IAttribute
{
	Attribute_Physics();
	~Attribute_Physics();

	AttributePtr<Attribute_Spatial> ptr_spatial;
	AttributePtr<Attribute_Render> ptr_render;

	Float3 linearVelocity;
	Float3 angularVelocity;
	Float3 gravity;
	float mass;
	unsigned int meshID; //collisionShapeIndex;
	short collisionFilterGroup;
	short collisionFilterMask;
	//CollisionShape
	//friction
	//restitution

	bool collisionResponse;
	bool reloadDataIntoBulletPhysics;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_PHYSICS;}
	std::string getName(){return "Physics";}
};

/// Stores everything GameComponent needs to know when handling 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Projectile : public IAttribute
{
	Attribute_Projectile();
	~Attribute_Projectile();

	AttributePtr<Attribute_Physics> ptr_physics;

	int entityIdOfCreator;		//!< Entity id of the entity that created the projectile.
	float totalLifeTime;
	float currentLifeTimeLeft;	//!< Counter counting down the lifetime of the projectile. Is initialized to totalLifeTime. When equal or less than zero, the projectile attribute shall be destroyed.
	XKILL_Enums::AmmunitionType ammunitionType;
	XKILL_Enums::FiringModeType firingModeType;
	bool scatterDropped;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_PROJECTILE;}
	std::string getName(){return "Projectile";}
};

/// Represents a directional light. The LightDescDir-struct is directly mapped to GPU.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Light_Dir : public IAttribute
{
	Attribute_Light_Dir();
	~Attribute_Light_Dir(); //!< Does nothing.

	Float3 direction;

	LightDescDir lightDir;
	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_LIGHT_DIRECTIONAL;}
	std::string getName(){return "LightDir";}
};
/// Represents a point-light. The LightDescPoint-struct is directly mapped to GPU.
/** 
OBS! The pos-attribute in LightDescPoint must be updated with the corresponding value in AttributePointer ptr_position-type in order to have the correct position! OBS!
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Light_Point : public IAttribute
{
	Attribute_Light_Point();
	~Attribute_Light_Point(); //!< Does nothing.

	AttributePtr<Attribute_Position> ptr_position; //!< The correct position of point-light.

	LightDescPoint lightPoint;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_LIGHT_POINT;}
	std::string getName(){return "LightPoint";}
};

struct DLL_U Attribute_Input : public IAttribute
{
	Attribute_Input();
	~Attribute_Input();

	AttributePtr<Attribute_Physics> ptr_physics;
	Float2 position;
	Float2 rotation;
	bool fire;
	bool firePressed;
	bool jump;
	bool jetpack;
	bool sprint;
	int changeAmmunitionType;
	int changeFiringMode;
	bool lowSensitivity;
	bool reload;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_INPUT;}
	std::string getName(){return "Input";}
};

class InputDevice;

/// Represent an input device
/**
\ingroup ATTRIBUTES
*/

struct DLL_U Attribute_InputDevice : public IAttribute
{
	Attribute_InputDevice();
	~Attribute_InputDevice();

	InputDevice* device;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_INPUTDEVICE;}
	std::string getName(){return "InputDevice";}
};

/// Stores everything SoundComponent needs to know to play a 3D sound
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Sound : public IAttribute
{
	Attribute_Sound();
	~Attribute_Sound();

	AttributePtr<Attribute_Position> ptr_position;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_SOUND;}
	std::string getName(){return "Sound";}
};

//class EventToFModConverter;

struct DLL_U Attribute_SoundSettings : public IAttribute
{
	Attribute_SoundSettings();
	~Attribute_SoundSettings();

	bool soundMuted;
	float soundVolume;

	//EventToFModConverter* converter;
	
	DataItemList* getDataList();
	void saveTo(DataItemList* list);

	AttributeType getType(){return ATTRIBUTE_SOUNDSETTINGS;}
	std::string getName(){return "SoundSettings";}
};

/// Stores everything RenderComponent needs to know to manage multiple Cameras in the world
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Camera : public IAttribute
{
	const static int attributeType = ATTRIBUTE_POSITION;

	Attribute_Camera();
	~Attribute_Camera();

	AttributePtr<Attribute_Spatial> ptr_spatial;
	AttributePtr<Behavior_Offset> ptr_offset;

	Float4x4 mat_view;			//!< The view matrix. Used to transform objects to view space.
	Float4x4 mat_projection;	//!< The projection matrix. Defines the camera's frustum.
	float fieldOfView;
	float fieldOfViewModifier;
	float aspectRatio;
	float zNear;
	float zFar;

	Float3 up;			//!< Always aims up from the camera, perpendicular to look.
	Float3 right;		//!< Always aims to the right of the camera, perpendicular to look.
	Float3 look;		//!< The direction in which the camera is aimed.

	void syncSpatialWithAim();
	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_CAMERA;}
	std::string getName(){return "Camera";}
};

struct Attribute_Player;
struct DLL_U Attribute_SplitScreen : public IAttribute
{
	Attribute_SplitScreen();
	~Attribute_SplitScreen();

	AttributePtr<Attribute_Camera> ptr_camera;
	AttributePtr<Attribute_Player> ptr_player;

	unsigned int ssTopLeftX;
	unsigned int ssTopLeftY;

	unsigned int ssWidth;
	unsigned int ssHeight;

	Float3 glowmod;

	float getAspectRatio();
	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_SPLITSCREEN;}
	std::string getName(){return "SplitScreen";}
};

struct Attribute_Health;
struct Attribute_WeaponStats;
/// Stores everything GameComponent needs to know about a player (also refer to createPlayerEntity)
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Player : public IAttribute
{
	Attribute_Player();
	~Attribute_Player();

	void clean();

	AttributePtr<Attribute_Render>			ptr_render;
	AttributePtr<Attribute_Input>			ptr_input;
	AttributePtr<Attribute_InputDevice>		ptr_inputDevice;
	AttributePtr<Attribute_Camera>			ptr_camera;
	AttributePtr<Attribute_Health>			ptr_health;
	AttributePtr<Attribute_WeaponStats>		ptr_weaponStats;
	AttributePtr<Behavior_Offset>			ptr_weapon_offset;
	AttributePtr<Behavior_Offset>			ptr_light_offset;
	AttributePtr<Attribute_Spatial>			ptr_weaponFireLocation_spatial;

	static int nextId;

	std::string avatarName;
	Float3 avatarColor;

	bool isScoreBoardVisible;
	bool showScoreboard;

	int priority;							//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;						//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int cycles;								//!< Total execution time of the player process, used ased final score in the deathmatch. The game session winner is the player with the most total execution time as awarded by the scheduler.
	float currentSpeed;						//!< Speed used when changing position in "handleInput".
	float walkSpeed;						//!< Speed when walking.
	float sprintSpeed;						//!< Speed when sprinting.
	float currentSprintTime;				//!< Sprinting time left
	float sprintTime;						//!< Time that can be spent sprinting
	bool canSprint;							//!< Can the player sprint right now
	float sprintRechargeRate;				//!< The rate at which the sprint will recharge
	Timer respawnTimer;						//!< Keeps track of when a dead player should respawn
	float timeSinceLastDamageTaken;			//!< Incrementing timer. Reset when taking damage.
	bool jetpack;							//!< Use jetpack
	bool detectedAsDead;					//!< The player is dead
	bool executing;							//!< True if selected by the scheduler (ScoreComponent.cpp)
	bool hovering;							//!< The player is in a stable hover and not falling or jumping or using jetpack
	std::pair<bool, Timer*> speedHackPair;	//!< The bool is true if the hack is active, the timer keeps track of time left
	std::pair<bool, Timer*> jetHackPair;	//!< The bool is true if the hack is active, the timer keeps track of time left
	std::pair<bool, Timer*> cycleHackPair;	//!< The bool is true if the hack is active, the timer keeps track of time left
	std::pair<bool, Timer*> powerHackPair;	//!< The bool is true if the hack is active, the timer keeps track of time left

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_PLAYER;}
	std::string getName(){return "Player";}
};


struct DLL_U Attribute_Mesh : public IAttribute
{
	unsigned int	meshID;		//!< ID of mesh, read from .mdldesc-file.
	MeshDesc		mesh;		//!< Type containing all mesh-related data.
	bool			dynamic;	//!< Whether or not mesh is supposed to be dynamic physics-wize.
	std::string		fileName;	//!< Filename of loaded model.
	VertexType		vertexType;

	void clean();					//!< Does nothing.
	Attribute_Mesh();				//!< Initializes attribute with default values. Dynamic = false.
	Attribute_Mesh(
		unsigned int	id,
		MeshDesc		mesh,
		bool			dynamic,
		std::string		fileName,
		VertexType		vertexType);	//!< Initializes attribute with passed values.
	~Attribute_Mesh();				//!< Does nothing.

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_MESH;}
	std::string getName(){return "Mesh";}
};

struct DLL_U Attribute_Health : public IAttribute
{
	Attribute_Health();
	~Attribute_Health();

	float maxHealth;
	float health;
	float healthFromLastFrame;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_HEALTH;}
	std::string getName(){return "Health";}
};

struct DLL_U Attribute_Damage : public IAttribute
{
	Attribute_Damage();
	~Attribute_Damage();

	float damage;
	int owner_entityID;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_DAMAGE;}
	std::string getName(){return "Damage";}
};

struct DLL_U Attribute_PlayerSpawnPoint : public IAttribute
{
	Attribute_PlayerSpawnPoint();
	~Attribute_PlayerSpawnPoint();

	AttributePtr<Attribute_Position> ptr_position;

	float secondsSinceLastSpawn;	//!< Is reset when a player spawns at the spawn point.
	float spawnArea;				//!< Defines the spawn point zone, a horizontal circle area (might have changed to sphere)

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_PLAYERSPAWNPOINT;}
	std::string getName(){return "PlayerSpawnPoint";}
};

struct DLL_U Attribute_PickupablesSpawnPoint : public IAttribute
{
	Attribute_PickupablesSpawnPoint();
	~Attribute_PickupablesSpawnPoint();

	AttributePtr<Attribute_Position> ptr_position;

	XKILL_Enums::PickupableType spawnPickupableType;	//!< Type of pickupable spawned by this pickupables spawn point.
	float spawnDelayInSeconds;							//!< Delay until a pickupable may spawn
	float secondsSinceLastSpawn;						//!< Incrementing timer, reset when spawned.
	float secondsSinceLastPickup;						//!< Incrementing timer, reset when picked up.
	int maxNrOfExistingSpawnedPickupables;				//!< Is checked against "currentNrOfExistingSpawnedPickupables"
	int currentNrOfExistingSpawnedPickupables;			//!< Incremented when a pickupable is spawned from this pickupables spawn point. Decremented when a pickupable is picked up.

	DataItemList* getDataList();
	void saveTo(DataItemList* list);

	AttributeType getType(){return ATTRIBUTE_PICKUPABLESSPAWNPOINT;}
	std::string getName(){return "PickupablesSpawnPoint";}
};

struct DLL_U Attribute_Pickupable : public IAttribute
{
	Attribute_Pickupable();
	~Attribute_Pickupable();

	AttributePtr<Attribute_Position> ptr_position;
	AttributePtr<Attribute_Physics> ptr_physics;
	AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint_creator;	//! The pickupable spawnpoint that spawned this pickupable

	XKILL_Enums::PickupableType pickupableType;			//! MEDKIT, AMMUNITION_BULLET, AMMUNITION_SCATTER, AMMUNITION_EXPLOSIVE, etc
	int amount;											//! Data of pickupable (health, ammo, etc) //Deprecated as of 2013-03-06 15.25. Amount is determined on pickup instead of at spawn time.
	Float3 getColor();

	DataItemList* getDataList();
	void saveTo(DataItemList* list);

	AttributeType getType(){return ATTRIBUTE_PICKUPABLE;}
	std::string getName(){return "Pickupable";}
};

class MutatorSettings;
#include "WeaponStructs.h"
/// Stores everything needed for the weapon system. The two enums "XKILL_Enums::AmmunitionType" and "XKILL_Enums::FiringModeType" is used to preset the weapon settings. These settings are used in GameComponent to simulate the weapon behavior of choice.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_WeaponStats : public IAttribute
{
	Attribute_WeaponStats();
	~Attribute_WeaponStats();

	Ammunition ammunition[XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES];
	FiringMode firingMode[XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES];

	XKILL_Enums::AmmunitionType currentAmmunitionType;
	XKILL_Enums::FiringModeType currentFiringModeType;

	void setWeaponStats(XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingModeType);

	std::string getAmmunitionTypeAsString();
	std::string getFiringModeAsString();

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_WEAPONSTATS;}
	std::string getName(){return "WeaponStats";}
};


struct DLL_U Attribute_ExplosionSphere : public IAttribute
{
	Attribute_ExplosionSphere();
	~Attribute_ExplosionSphere();

	AttributePtr<Attribute_Physics> ptr_physics;
	float damage;
	float currentLifeTimeLeft;
	float currentRadius;							//!< Incremented in ExplosionSpherePhysicsObject::onUpdate
	XKILL_Enums::AmmunitionType ammunitionType;
	XKILL_Enums::FiringModeType firingModeType;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_EXPLOSIONSPHERE;}
	std::string getName(){return "ExplosionSphere";}
};

struct DLL_U Attribute_Ray : public IAttribute
{
	Attribute_Ray();
	~Attribute_Ray();

	AttributePtr<Attribute_Render> ptr_render;

	Float3 from;
	Float3 to;
	bool render;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);

	AttributeType getType(){return ATTRIBUTE_RAY;}
	std::string getName(){return "Ray";}
};