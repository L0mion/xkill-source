#pragma once

#include "dllUtilities.h"
#include "AttributePointer.h"


/// Used inside \ref components for data processing 
/** 
Created by AttributeManager and stored in a AttributeStorage.

An attribute is basically just a Struct which holds Data, however 
each attribute can be shared among multiple Components which allows
each Component to work with the same data independently of eachother
with minimal overhead.

The attributes listed is currently only placeholders and should
be modified to suit the need of each Component.

\defgroup attributes Attributes
\ingroup utilities
*/

/// Attribute interface to facilitate grouping of attributes.
/** 
\ingroup attributes
*/
struct DLL_U IAttribute
{
};


///////////////////////////////////////////
// Attributes
///////////////////////////////////////////

/// Stores the position of an Entity 
/** 
\ingroup attributes
*/
struct DLL_U PositionAttribute : public IAttribute
{
	float position;
};

/// Stores detailed Spatial informaiton about an Entity 
/** 
\ingroup attributes
*/
struct DLL_U SpatialAttribute : public IAttribute
{
	AttributePointer positionAttribute;

	float rotation;
	float scale;
};

/// Stores everything RenderComponent needs to know about an entity
/** 
A good approach for the RenderComponent would be to step through all 
RenderAttribute and construct multiple queues consisting of objects
that should be transparent or not, tesselated or not, use the same
meshID and textureID, and the Render each Queue in a orderly fashion 
throught the use of Instancing.

\ingroup attributes
*/
struct DLL_U RenderAttribute : public IAttribute
{
	AttributePointer spatialAttribute;

	bool transparent;
	bool tessellation;
	int meshID;
	int textureID;
};

/// Stores everything PhysicsComponent needs to know about an entity
/** 
\ingroup attributes
*/
struct DLL_U PhysicsAttribute : public IAttribute
{
	AttributePointer spatialAttribute;
	float velocity[4];
	float rotationVelocity[4];
	float mass;

	bool added;
	bool alive;
};

/// Stores everything SoundComponent needs to know to play a 3D sound
/** 
\ingroup attributes
*/
struct DLL_U SoundAttribute : public IAttribute
{
	AttributePointer positionAttribute;
};

/// Stores everything RenderComponent needs to know to manage multiple Cameras in the world
/** 
\ingroup attributes
*/
struct DLL_U CameraAttribute : public IAttribute
{
	AttributePointer spatialAttribute;
	float mat_viewProj;
	float fieldOfView;
};