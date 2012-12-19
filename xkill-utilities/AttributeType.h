#pragma once

#include "dllUtilities.h"
#include "AttributePointer.h"
#include "Math.h"
#include <string>


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
	virtual void clean(){};
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
	ATTRIBUTE_PLAYER,
	ATTRIBUTE_BOUNDING,
	ATTRIBUTE_MESH,
	ATTRIBUTE_PROJECTILE,

	ATTRIBUTE_HEALTH,
	ATTRIBUTE_DAMAGE,
	ATTRIBUTE_SPAWNPOINT,
	ATTRIBUTE_WEAPONSTATS,

	// this is needed, don't touch!
	ATTRIBUTE_LAST
};

/// Stores the position of an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U PositionAttribute : public IAttribute
{
	PositionAttribute();
	~PositionAttribute();
	Float3 position;
};

/// Stores detailed Spatial informaiton about an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U SpatialAttribute : public IAttribute
{
	SpatialAttribute();
	~SpatialAttribute();

	AttributePointer positionAttribute;

	Float4 rotation;
	Float3 scale;
};

/// Stores the points for both a bounding box and a convex mesh
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U BoundingAttribute : public IAttribute
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
struct DLL_U RenderAttribute : public IAttribute
{
	RenderAttribute();
	~RenderAttribute();

	AttributePointer spatialAttribute;
	AttributePointer boundingAttribute;
	
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
struct DLL_U PhysicsAttribute : public IAttribute
{
	PhysicsAttribute();
	~PhysicsAttribute();

	AttributePointer spatialAttribute;
	AttributePointer renderAttribute;
	Float3 linearVelocity;
	Float3 angularVelocity;
	Float3 gravity;
	float mass;
	unsigned int meshID; //collisionShapeIndex;
	//CollisionShape
	//friction
	//restitution

	bool collisionResponse;
	bool added;
	bool alive;
	bool isProjectile;

	bool isExplosionSphere;
	float explosionSphereRadius;
};

/// Stores everything GameComponent needs to know when handling 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U ProjectileAttribute : public IAttribute
{
	ProjectileAttribute();
	~ProjectileAttribute();

	AttributePointer physicsAttribute;
	int entityIdOfCreator;		//!< Entity id of the entity that created the projectile.
	float currentLifeTimeLeft;	//!< Counter counting down the lifetime of the projectile. Is initialized to totalLifeTime. When equal or less than zero, the projectile attribute shall be destroyed.
	bool explodeOnImnpact;
	float explosionSphereRadius;
};

struct DLL_U InputAttribute : public IAttribute
{
	InputAttribute();
	~InputAttribute();

	AttributePointer physicsAttribute;
	Float2 position;
	Float2 rotation;
	bool fire;
	bool changeWeapon;
};

/// Stores everything SoundComponent needs to know to play a 3D sound
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U SoundAttribute : public IAttribute
{
	SoundAttribute();
	~SoundAttribute();

	AttributePointer positionAttribute;
};

/// Stores everything RenderComponent needs to know to manage multiple Cameras in the world
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U CameraAttribute : public IAttribute
{
	CameraAttribute();
	~CameraAttribute();
	AttributePointer spatialAttribute;
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
struct DLL_U PlayerAttribute : public IAttribute
{
	PlayerAttribute();
	~PlayerAttribute();

	int id;					//!< The id of the player process. Used to identify a player attribute in GameComponent when firing projectiles.
	int priority;			//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;		//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int totalExecutionTime; //!< Total execution time of the player process, used ased final score in the deathmatch. The game session winner is the player with the most total execution time as awarded by the scheduler.

	AttributePointer renderAttribute;
	AttributePointer inputAttribute;
	AttributePointer cameraAttribute;
	AttributePointer healthAttribute;
	AttributePointer weaponStatsAttribute;
};


class MeshModel;
struct DLL_U MeshAttribute : public IAttribute
{
	unsigned int	meshID;		//!< ID of mesh, read from .mdldesc-file.
	MeshModel*		mesh;		//!< Type containing all mesh-related data.
	bool			dynamic;	//!< Whether or not mesh is supposed to be dynamic physics-wize.

	void clean();					//!< Does nothing.
	MeshAttribute();				//!< Initializes attribute with default values. Dynamic = false.
	MeshAttribute(
		unsigned int	id,
		MeshModel*		mesh,
		bool			dynamic);	//!< Initializes attribute with passed values.
	~MeshAttribute();				//!< Does nothing.
};

struct DLL_U HealthAttribute : public IAttribute
{
	HealthAttribute();
	~HealthAttribute();

	int startHealth;
	int health;
};

struct DLL_U DamageAttribute : public IAttribute
{
	DamageAttribute();
	~DamageAttribute();

	int damage;
	int owner_entityID;
};

struct DLL_U SpawnPointAttribute : public IAttribute
{
	SpawnPointAttribute();
	~SpawnPointAttribute();

	float timeSinceLastSpawn;	//!< Is reset when a player spawns at the spawn point.
	float spawnArea;			//!< Defines the spawn point zone, a horizontal circle area.
	AttributePointer positionAttribute;
};

/// Stores everything needed for the weapon system. The two enums "AmmunitionType" and "FiringMode" is used to preset the weapon settings. These settings are used in GameComponent to simulate the weapon behavior of choice.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U WeaponStatsAttribute : public IAttribute
{
	enum AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE
	};

	enum FiringMode
	{
		SINGLE,
		SEMI,
		AUTO
	};

	WeaponStatsAttribute();
	~WeaponStatsAttribute();

	void setWeaponStats(AmmunitionType ammunitionType, FiringMode firingMode);
	void setWeaponToDebugMachineGun();

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
struct DLL_U DebugShapeAttribute : public IAttribute
{
	DebugShapeAttribute();
	~DebugShapeAttribute();
	void clean();

	AttributePointer spatialAttribute;

	DebugShape* shape;
	bool		render;
};