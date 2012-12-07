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
	ATTRIBUTE_PHYSICS,
	ATTRIBUTE_CAMERA,
	ATTRIBUTE_INPUT,
	ATTRIBUTE_PLAYER,
	ATTRIBUTE_BOUNDING,
	ATTRIBUTE_MESH,
	ATTRIBUTE_PROJECTILE,

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
throught the use of Instancing.

\ingroup ATTRIBUTES
*/
struct DLL_U RenderAttribute : public IAttribute
{
	RenderAttribute();
	~RenderAttribute();

	AttributePointer spatialAttribute;
	AttributePointer boundingAttribute;
	
	bool transparent;
	bool tessellation;
	int meshIndex;
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
	Float3 linearVelocity;
	Float3 angularVelocity;
	float mass;
	unsigned int collisionShapeIndex;
	//CollisionShape
	//friction
	//restitution

	bool added;
	bool alive;
	bool isProjectile;
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
	int creator;				//!< Entity id of the entity that created the projectile.
	float currentLifeTimeLeft;	//!< Counter counting down the lifetime of the projectile. Is initialized to totalLifeTime. When equal or less than zero, the projectile attribute shall be destroyed.
};

struct DLL_U InputAttribute : public IAttribute
{
	InputAttribute();
	~InputAttribute();

	AttributePointer physicsAttribute;
	Float2 position;
	Float2 rotation;
	bool fire;
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
	std::string name;		//!< Name of the player process.
	int health;				//!< Health of the player process. If less or equal to zero, the player process is terminated.
	int priority;			//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;		//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int totalExecutionTime; //!< Total execution time of the player process. The game session winner is the player with the most total execution time as awarded by the scheduler.

	AttributePointer renderAttribute;
	AttributePointer inputAttribute;
	AttributePointer cameraAttribute;
};


class MeshModel;
struct DLL_U MeshAttribute : public IAttribute
{
	MeshModel* mesh;
	bool dynamic;

	void clean();
	MeshAttribute(){};
	~MeshAttribute();;
};
