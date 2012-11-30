#pragma once

#include "dllUtilities.h"
#include "AttributePointer.h"
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
	virtual ~IAttribute();
};


///////////////////////////////////////////
// Attributes
///////////////////////////////////////////

/// Stores the position of an Entity 
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U PositionAttribute : public IAttribute
{
	PositionAttribute();
	~PositionAttribute();
	float position[3];
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

	float rotation[4];
	float scale[3];
};

struct DLL_U InputAttribute : public IAttribute
{
	InputAttribute();
	~InputAttribute();
	float position[3];
	float rotation[2];
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
	float velocity[4];
	float rotationVelocity[4];
	float mass;

	bool added;
	bool alive;
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
	float mat_view[16];
	float mat_projection[16];
};

/// Stores everything GameComponent needs to know about a player
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U PlayerAttribute : public IAttribute
{
	PlayerAttribute();
	~PlayerAttribute();

	int playerID;
	std::string name;		//!< Name of the player process.
	int health;				//!< Health of the player process. If less or equal to zero, the player process is terminated.
	int priority;			//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;		//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int totalExecutionTime; //!< Total execution time of the player process. The game session winner is the player with the most total execution time as awared by the scheduler.

	int cameraId;			//!< The camera id of the player process.
	AttributePointer renderAttribute;
};