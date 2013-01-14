#pragma once

#include "dllUtilities.h"
#include "AttributePointer.h"
#include "Math.h"
#include "LightDesc.h"
#include <string>

#include "DataItem.h"


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

/// Attribute interface to facilitate grouping of attributes.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U IAttribute
{
	IAttribute();

	//! Called when deleting attributes (refer to AttributeStorage::deleteAttribute(int index))
	virtual void clean(){};

	virtual DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		return list;
	};

	virtual ~IAttribute();
};


///////////////////////////////////////////
// Attributes
///////////////////////////////////////////

// Enums over each Attribute Type
/*
Just something that can be used if "casting" of
\ref ATTRIBTUES is needed.
NOTE: DOXYGEN can not detect Enums combined with DLL
for some reason
*/


enum DLL_U AttributeType
{
	ATTRIBUTE_POSITION,
	ATTRIBUTE_SPATIAL,
	ATTRIBUTE_RENDER,
	ATTRIBUTE_DEBUGSHAPE,
	ATTRIBUTE_PHYSICS,
	ATTRIBUTE_CAMERA,
	ATTRIBUTE_INPUT,
	//ATTRIBUTE_INPUTDEVICESETTINGS,
	ATTRIBUTE_PLAYER,
	ATTRIBUTE_BOUNDING,
	ATTRIBUTE_MESH,
	ATTRIBUTE_PROJECTILE,
	
	ATTRIBUTE_LIGHT_DIRECTIONAL,
	ATTRIBUTE_LIGHT_POINT,
	ATTRIBUTE_LIGHT_SPOT,

	ATTRIBUTE_HEALTH,
	ATTRIBUTE_DAMAGE,
	ATTRIBUTE_SPAWNPOINT,
	ATTRIBUTE_WEAPONSTATS,
	ATTRIBUTE_EXPLOSIONSPHERE,

	// this is needed, don't touch!
	ATTRIBUTE_LAST
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add(position, "Position");
		return list;
	}
};

/// Stores detailed Spatial informaiton about an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Spatial : public IAttribute
{
	Attribute_Spatial();
	~Attribute_Spatial();

	AttributePointer ptr_position;

	Float4 rotation;
	Float3 scale;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add(rotation, "Rotation");
		list->add(scale,	"Scale");
		return list;
	}
};

/// Stores the points for both a bounding box and a convex mesh
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Bounding : public IAttribute
{
	float BoxPoints[8*3];
	float ConvexPoints[42*3];
};



/// Stores everything RenderComponent needs to know about an entity
/** 
A good approach for the RenderComponent would be to step through all 
RenderAttribute and construct multiple queues consisting of objects
that should be transparent or not, tesselated or not, use the same
meshID and textureID, and the Render each Queue in a orderly fashion 
through the use of Instancing.

\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Render : public IAttribute
{
	Attribute_Render();
	~Attribute_Render();

	AttributePointer ptr_spatial;
	AttributePointer ptr_bounding;
	
	BoolField culling;

	bool transparent;
	bool tessellation;

	int meshID;
	int textureID;
};

/// Stores everything PhysicsComponent needs to know about an entity
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Physics : public IAttribute
{
	enum PhysicsAttributeType
	{
		DEFAULT_ERROR = 0,
		WORLD = 1,
		PLAYER = 2,
		PROJECTILE = 4,
		EXPLOSIONSPHERE = 8,
		EVERYTHING = 0xffff
	};
	PhysicsAttributeType collisionFilterGroup;

	Attribute_Physics();
	~Attribute_Physics();

	AttributePointer ptr_spatial;
	AttributePointer ptr_render;

	Float3 linearVelocity;
	Float3 angularVelocity;
	Float3 gravity;
	float mass;
	unsigned int meshID; //collisionShapeIndex;
	short int collisionFilterMask;
	//CollisionShape
	//friction
	//restitution

	bool collisionResponse;
	bool added;
	bool alive;

	float explosionSphereRadius;
};

/// Stores everything GameComponent needs to know when handling 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Projectile : public IAttribute
{
	Attribute_Projectile();
	~Attribute_Projectile();

	AttributePointer ptr_physics;

	int entityIdOfCreator;		//!< Entity id of the entity that created the projectile.
	float currentLifeTimeLeft;	//!< Counter counting down the lifetime of the projectile. Is initialized to totalLifeTime. When equal or less than zero, the projectile attribute shall be destroyed.
	bool explodeOnImnpact;
	float explosionSphereRadius;
};

//struct DLL_U Attribute_Light : public IAttribute
//{
//	Attribute_Light();
//	~Attribute_Light(); //!< Does nothing.
//
//	LightType lightType; //!< Type of light: Directional, Point or Spot.
//
//	AttributePointer ptr_position; //!< Position of light.
//
//	Float3 direction;	//!< The lights direction.
//	Float3 attenuation;	//!< How fast the light intensity will diminish
//	
//	Float4 ambient;		//!< The ambient color.
//	Float4 diffuse;		//!< The diffuse color.
//	Float4 specular;	//!< The specular color.
//
//	float range;		//!< How far the light can reach.
//	float spotPower;	//!< Controls the spotlight cone.
//};

struct DLL_U Attribute_Light_Dir : public IAttribute
{
	Attribute_Light_Dir();
	~Attribute_Light_Dir();

	LightDescDir lightDir;
};
struct DLL_U Attribute_Light_Point : public IAttribute
{
	Attribute_Light_Point();
	~Attribute_Light_Point();

	AttributePointer ptr_position;

	LightDescPoint lightPoint;
};
struct DLL_U Attribute_Light_Spot : public IAttribute
{
	Attribute_Light_Spot();
	~Attribute_Light_Spot();

	AttributePointer ptr_position;

	LightDescSpot lightSpot;
};

struct DLL_U Attribute_Input : public IAttribute
{
	Attribute_Input();
	~Attribute_Input();

	AttributePointer ptr_physics;
	Float2 position;
	Float2 rotation;
	bool fire;
	bool changeAmmunitionType;
	bool changeFiringMode;
};

class InputObjectArray;

/// Stores the input objects of a input device
/**
\ingroup ATTRIBUTES
*/

//struct DLL_U Attribute_InputDeviceSettings : public IAttribute
//{
//	Attribute_InputDeviceSettings();
//	~Attribute_InputDeviceSettings();
//
//	InputObjectArray* inputObjectArray;
//};

/// Stores everything SoundComponent needs to know to play a 3D sound
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Sound : public IAttribute
{
	Attribute_Sound();
	~Attribute_Sound();

	AttributePointer ptr_position;
};

/// Stores everything RenderComponent needs to know to manage multiple Cameras in the world
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Camera : public IAttribute
{
	Attribute_Camera();
	~Attribute_Camera();

	AttributePointer ptr_spatial;

	Float4x4 mat_view;
	Float4x4 mat_projection;
	float fov;
	float aspect;
	float zNear;
	float zFar;
};

/// Stores everything GameComponent needs to know about a player (also refer to createPlayerEntity)
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Player : public IAttribute
{
	Attribute_Player();
	~Attribute_Player();

	int id;					//!< The id of the player process. Used to identify a player attribute in GameComponent when firing projectiles.
	int priority;			//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;		//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int totalExecutionTime; //!< Total execution time of the player process, used ased final score in the deathmatch. The game session winner is the player with the most total execution time as awarded by the scheduler.

	AttributePointer ptr_render;
	AttributePointer ptr_input;
	AttributePointer ptr_camera;
	AttributePointer ptr_health;
	AttributePointer ptr_weaponStats;
};


class MeshModel;
struct DLL_U Attribute_Mesh : public IAttribute
{
	unsigned int	meshID;		//!< ID of mesh, read from .mdldesc-file.
	MeshModel*		mesh;		//!< Type containing all mesh-related data.
	bool			dynamic;	//!< Whether or not mesh is supposed to be dynamic physics-wize.

	void clean();					//!< Does nothing.
	Attribute_Mesh();				//!< Initializes attribute with default values. Dynamic = false.
	Attribute_Mesh(
		unsigned int	id,
		MeshModel*		mesh,
		bool			dynamic);	//!< Initializes attribute with passed values.
	~Attribute_Mesh();				//!< Does nothing.
};

struct DLL_U Attribute_Health : public IAttribute
{
	Attribute_Health();
	~Attribute_Health();

	float startHealth;
	float health;
};

struct DLL_U Attribute_Damage : public IAttribute
{
	Attribute_Damage();
	~Attribute_Damage();

	float damage;
	int owner_entityID;
};

struct DLL_U Attribute_SpawnPoint : public IAttribute
{
	Attribute_SpawnPoint();
	~Attribute_SpawnPoint();

	float timeSinceLastSpawn;	//!< Is reset when a player spawns at the spawn point.
	float spawnArea;			//!< Defines the spawn point zone, a horizontal circle area.

	AttributePointer ptr_position;
};

/// Stores everything needed for the weapon system. The two enums "AmmunitionType" and "FiringMode" is used to preset the weapon settings. These settings are used in GameComponent to simulate the weapon behavior of choice.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_WeaponStats : public IAttribute
{
	enum AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE,

		NROFAMUNITIONTYPES
	};

	enum FiringMode
	{
		SINGLE,
		SEMI,
		AUTO,

		NROFFIRINGMODES
	};

	Attribute_WeaponStats();
	~Attribute_WeaponStats();

	void setWeaponStats(AmmunitionType ammunitionType, FiringMode firingMode);
	void setWeaponToDebugMachineGun();
	std::string getAmmunitionTypeAsString();
	std::string getFiringModeAsString();

	AmmunitionType ammunitionType;	//!< BULLET, SCATTER, EXPLOSIVE
	FiringMode firingMode;			//!< SINGLE, SEMI, AUTO

	int totalNrOfShots;				//!< Total number of shots that can be fired. A value of -1 denotes unlimited ammunition.
	int clipSize;					//!< Number of shots that can be fired before reload is needed. A value of 0 denotes that no reload is necessary.
	int nrOfShotsLeftInClip;		//!< Current number of shots left in the current clip.

	float reloadTime;				//!< Number of seconds it takes to reload.
	float reloadTimeLeft;			//!< Reload progress, when lesser or equal to 0 the weapon is reloaded.
	float cooldownBetweenShots;		//!< Number of seconds that must pass between each shot.
	float cooldownLeft;				//!< Number of seconds until a new shot can be fired.

	float velocityOfEachProjectile; //!< Velocity of the PhysicsAttribute when creating a projectile.
	int nrOfProjectilesForEachShot; //!< If > 1 then scattershot else singleshot.
	float damgeOfEachProjectile;	//!< Damage value of the damage attribute created when creating a projectile from this weapon.

	float displacementSphereRadius;	//!< Randomizes the position of each projectile inside this sphere.
	float spreadConeRadius;			//!< Randomizes the orientation of each projectile's velocity vector inside this cone. 

	bool isExplosive;				//!< Determines if projectiles created from this weapon will explode on impact.
	float explosionSphereRadius;	//!< Radius of explosion sphere.
};

struct DebugShape;
struct DLL_U Attribute_DebugShape : public IAttribute
{
	Attribute_DebugShape();
	~Attribute_DebugShape();
	void clean();

	unsigned int	meshID;		//!< ID of mesh
	AttributePointer ptr_spatial;

	DebugShape* shape;
	bool		render;
};

struct DLL_U Attribute_ExplosionSphere : public IAttribute
{
	Attribute_ExplosionSphere();
	~Attribute_ExplosionSphere();

	AttributePointer ptr_physics;
	float currentLifeTimeLeft;
};