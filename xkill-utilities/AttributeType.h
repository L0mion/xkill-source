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
	ATTRIBUTE_UNKOWN,

	ATTRIBUTE_POSITION,
	ATTRIBUTE_SPATIAL,
	ATTRIBUTE_RENDER,
	ATTRIBUTE_DEBUGSHAPE,
	ATTRIBUTE_PHYSICS,
	ATTRIBUTE_CAMERA,
	ATTRIBUTE_SPLITSCREEN,
	ATTRIBUTE_INPUT,
	ATTRIBUTE_INPUTDEVICE,
	ATTRIBUTE_SOUND,

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

	virtual DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_NotSupported("NOT_IMPLEMENTED");
		return list;
	};
	virtual void saveTo(DataItemList* list)
	{
		// DO NOTHING
	};
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add(position, "position");
		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get(&position);
	};
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_AttributePointer(ptr_position.index,	"ptr_position");
		list->add(rotation,								"rotation");
		list->add(scale,								"scale");
		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		list->get(&rotation);
		list->get(&scale);
	};
	AttributeType getType(){return ATTRIBUTE_SPATIAL;}
	std::string getName(){return "Spatial";}
};

/// Stores the points for both a bounding box and a convex mesh
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Bounding : public IAttribute
{
	float BoxPoints[8*3];
	float ConvexPoints[42*3];
	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_NotSupported("boxPoints");
		list->add_NotSupported("bonvexPoints");
		return list;
	}
	AttributeType getType(){return ATTRIBUTE_BOUNDING;}
	std::string getName(){return "Bounding";}
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
	
	int meshID;
	int textureID;

	bool transparent;
	bool tessellation;

	

	BoolField culling;
	bool cull;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_AttributePointer(ptr_spatial.index,	"ptr_spatial");
		list->add_AttributePointer(ptr_bounding.index,	"ptr_bounding");
		list->add(meshID,								"meshID");
		list->add(textureID,							"textureID");
		list->add(transparent,							"transparent");
		list->add(tessellation,							"tessellation");
		list->add(culling.values[0],					"culling1");
		list->add(culling.values[1],					"culling2");
		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_spatial.index);
		list->get_AttributePointer(&ptr_bounding.index);
		list->get(&meshID);
		list->get(&textureID);
		list->get(&transparent);
		list->get(&tessellation);
		list->get(&culling.values[0]);
		list->get(&culling.values[1]);
	};
	AttributeType getType(){return ATTRIBUTE_RENDER;}
	std::string getName(){return "Render";}
};

/// Stores everything PhysicsComponent needs to know about an entity
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Physics : public IAttribute
{
	enum PhysicsAttributeType
	{
		NOTHING = 0,
		WORLD = 1,
		PLAYER = 2,
		PROJECTILE = 4,
		EXPLOSIONSPHERE = 8,
		FRUSTUM = 16,
		PICKUPABLE = 32,
		EVERYTHING = -1
	};
	PhysicsAttributeType collisionFilterGroup;

	Attribute_Physics();
	~Attribute_Physics();

	AttributePtr<Attribute_Spatial> ptr_spatial;
	AttributePtr<Attribute_Render> ptr_render;

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
	bool reloadDataIntoBulletPhysics;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_AttributePointer(ptr_spatial.index,	"ptr_spatial");
		list->add_AttributePointer(ptr_render.index,	"ptr_render");
		list->add(linearVelocity,						"linearVelocity");
		list->add(angularVelocity,						"angularVelocity");
		list->add(gravity,								"gravity");
		list->add(mass,									"mass");
		list->add(meshID,								"meshID");
		list->add(collisionFilterMask,					"collisionFilterMask");
		list->add(collisionResponse,					"collisionResponse");
		list->add(reloadDataIntoBulletPhysics,			"reloadDataIntoBulletPhysics");
		
		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_spatial.index);
		list->get_AttributePointer(&ptr_render.index);
		list->get(&linearVelocity);
		list->get(&angularVelocity);
		list->get(&gravity);
		list->get(&mass);
		list->get(&meshID);
		list->get(&collisionFilterMask);
		list->get(&collisionResponse);
		list->get(&reloadDataIntoBulletPhysics);
	};
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
	float currentLifeTimeLeft;	//!< Counter counting down the lifetime of the projectile. Is initialized to totalLifeTime. When equal or less than zero, the projectile attribute shall be destroyed.
	bool explodeOnImnpact;
	float explosionSphereRadius;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_AttributePointer(ptr_physics.index,	"ptr_physics");
		list->add(entityIdOfCreator,					"entityIdOfCreator");
		list->add(currentLifeTimeLeft,					"currentLifeTimeLeft");
		list->add(explodeOnImnpact,						"explodeOnImnpact");
		list->add(explosionSphereRadius,				"explosionSphereRadius");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_physics.index);
		list->get(&entityIdOfCreator);
		list->get(&currentLifeTimeLeft);
		list->get(&explodeOnImnpact);
		list->get(&explosionSphereRadius);
	};
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

	LightDescDir lightDir;
	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add(lightDir.ambient,		"ambient");
		list->add(lightDir.diffuse,		"diffuse");
		list->add(lightDir.specular,	"specular");
		list->add(lightDir.direction,	"direction");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get(&lightDir.ambient);
		list->get(&lightDir.diffuse);
		list->get(&lightDir.specular);
		list->get(&lightDir.direction);
	};
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_AttributePointer	(ptr_position.index, "ptr_position");
		list->add(lightPoint.ambient,		"ambient");
		list->add(lightPoint.diffuse,		"diffuse");
		list->add(lightPoint.specular,		"specular");
		//list->add(lightPoint.pos,			"pos");
		list->add(lightPoint.range,			"range");
		list->add(lightPoint.attenuation,	"attenuation");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		list->get(&lightPoint.ambient);
		list->get(&lightPoint.diffuse);
		list->get(&lightPoint.specular);
		//list->get(&lightPoint.pos);
		list->get(&lightPoint.range);
		list->get(&lightPoint.attenuation);
	};
	AttributeType getType(){return ATTRIBUTE_LIGHT_POINT;}
	std::string getName(){return "LightPoint";}
};
/// Represents a spot-light. The LightDescSpot-struct is directly mapped to GPU.
/** 
OBS! The pos-attribute in LightDescSpot must be updated with the corresponding value in AttributePointer ptr_position-type in order to have the correct position! OBS!
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_Light_Spot : public IAttribute
{
	Attribute_Light_Spot();
	~Attribute_Light_Spot(); //!< Does nothing.

	AttributePtr<Attribute_Position> ptr_position; //!< The correct position of spotlight.

	LightDescSpot lightSpot;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_AttributePointer	(ptr_position.index, "ptr_position");
		list->add(lightSpot.ambient,		"ambient");
		list->add(lightSpot.diffuse,		"diffuse");
		list->add(lightSpot.specular,		"specular");
		//list->add(lightSpot.pos,			"pos");
		list->add(lightSpot.range,			"range");
		list->add(lightSpot.direction,		"direction");
		list->add(lightSpot.spotPow,		"spotPow");
		list->add(lightSpot.attenuation,	"attenuation");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		list->get(&lightSpot.ambient);
		list->get(&lightSpot.diffuse);
		list->get(&lightSpot.specular);
		//list->get(&lightSpot.pos);
		list->get(&lightSpot.range);
		list->get(&lightSpot.direction);
		list->get(&lightSpot.spotPow);
		list->get(&lightSpot.attenuation);
	};
	AttributeType getType(){return ATTRIBUTE_LIGHT_SPOT;}
	std::string getName(){return "LightSpot";}
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
	bool killPlayer;
	bool changeAmmunitionType;
	bool changeFiringMode;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_AttributePointer(ptr_physics.index, "ptr_physics");
		list->add(position,					"position");
		list->add(rotation,					"rotation");
		list->add(fire,						"fire");
		list->add(changeAmmunitionType,		"changeAmmunitionType");
		list->add(changeFiringMode,			"changeFiringMode");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_physics.index);
		list->get(&position);
		list->get(&rotation);
		list->get(&fire);
		list->get(&changeAmmunitionType);
		list->get(&changeFiringMode);
	};
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_NotSupported("device");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_NotSupported();
	};
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_AttributePointer(ptr_position.index, "ptr_position");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
	};
	AttributeType getType(){return ATTRIBUTE_SOUND;}
	std::string getName(){return "Sound";}
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

	Float4x4 mat_view;			//!< The view matrix. Used to transform objects to view space.
	Float4x4 mat_projection;	//!< The projection matrix. Defines the camera's frustum.
	float fieldOfView;
	float aspectRatio;
	float zNear;
	float zFar;

	Float3 up;			//!< Always aims up from the camera, perpendicular to look.
	Float3 right;		//!< Always aims to the right of the camera, perpendicular to look.
	Float3 look;		//!< The direction in which the camera is aimed.

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add_AttributePointer(ptr_spatial.index,	"ptr_spatial");
		list->add(mat_view,								"mat_view");
		list->add(mat_projection,						"mat_projection");
		list->add(fieldOfView,							"fieldOfView");
		list->add(aspectRatio,							"aspectRatio");
		list->add(zNear,								"zNear");
		list->add(zFar,									"zFar");
		list->add(up,									"up");
		list->add(right,								"right");
		list->add(look,									"look");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_spatial.index);
		list->get(&mat_view);
		list->get(&mat_projection);
		list->get(&fieldOfView);
		list->get(&aspectRatio);
		list->get(&zNear);
		list->get(&zFar);
		list->get(&up);
		list->get(&right);
		list->get(&look);
	};
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

	float getAspectRatio()
	{
		return (float)ssWidth/(float)ssHeight;
	}
	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add_AttributePointer(ptr_camera.index,	"ptr_camera");
		list->add_AttributePointer(ptr_player.index,	"ptr_player");
		list->add(ssTopLeftX,							"ssTopLeftX");
		list->add(ssTopLeftY,							"ssTopLeftY");
		list->add(ssWidth,								"ssWidth");
		list->add(ssHeight,								"ssHeight");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_camera.index);
		list->get_AttributePointer(&ptr_player.index);
		list->get(&ssTopLeftX);
		list->get(&ssTopLeftY);
		list->get(&ssWidth);
		list->get(&ssHeight);
	};
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

	AttributePtr<Attribute_Render> ptr_render;
	AttributePtr<Attribute_Input> ptr_input;
	AttributePtr<Attribute_InputDevice> ptr_inputDevice;
	AttributePtr<Attribute_Camera> ptr_camera;
	AttributePtr<Attribute_Health> ptr_health;
	AttributePtr<Attribute_WeaponStats> ptr_weaponStats;

	static int nextId;

	int id;						//!< The id of the player process. Used to identify a player attribute in GameComponent when firing projectiles.
	int priority;				//!< Priority of the player process. Higher value means higher priority. The scheduler will choose the process with the highest priority for execution.
	int cycleSteals;			//!< Total number of cycle steals for the player process. Cycle steals steal priority from other player processes.
	int totalExecutionTime;		//!< Total execution time of the player process, used ased final score in the deathmatch. The game session winner is the player with the most total execution time as awarded by the scheduler.
	float currentSpeed;			//!< Speed used when changing position in "handleInput".
	float walkSpeed;			//!< Speed when walking.
	float sprintSpeed;			//!< Speed when sprinting.
	float respawnDelay;			//!< Time between death and respawn
	float currentRespawnDelay;	//!< Time until respawn
	float timeSinceLastJump;	//!< Incrementing timer

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		
		list->add_AttributePointer(ptr_render.index,		"ptr_render");
		list->add_AttributePointer(ptr_input.index,			"ptr_input");
		list->add_AttributePointer(ptr_camera.index,		"ptr_camera");
		list->add_AttributePointer(ptr_health.index,		"ptr_health");
		list->add_AttributePointer(ptr_weaponStats.index,	"ptr_weaponStats");
		list->add(id,					"id");
		list->add(priority,				"priority");
		list->add(cycleSteals,			"cycleSteals");
		list->add(totalExecutionTime,	"totalExecutionTime");
		list->add(currentSpeed,			"priority");
		list->add(walkSpeed,			"walkSpeed");
		list->add(sprintSpeed,			"sprintSpeed");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_render.index);
		list->get_AttributePointer(&ptr_input.index);
		list->get_AttributePointer(&ptr_camera.index);
		list->get_AttributePointer(&ptr_health.index);
		list->get_AttributePointer(&ptr_weaponStats.index);
		list->get(&id);
		list->get(&priority);
		list->get(&cycleSteals);
		list->get(&totalExecutionTime);
		list->get(&currentSpeed);
		list->get(&walkSpeed);
		list->get(&sprintSpeed);
	};
	AttributeType getType(){return ATTRIBUTE_PLAYER;}
	std::string getName(){return "Player";}
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

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
	
		list->add(meshID,		"meshID");
		list->add_NotSupported(	"mesh");
		list->add(dynamic,		"dynamic");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get(&meshID);
		list->get_NotSupported();
		list->get(&dynamic);
	};
	AttributeType getType(){return ATTRIBUTE_MESH;}
	std::string getName(){return "Mesh";}
};

struct DLL_U Attribute_Health : public IAttribute
{
	Attribute_Health();
	~Attribute_Health();

	float startHealth;
	float health;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add(startHealth,	"startHealth");
		list->add(health,		"health");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get(&startHealth);
		list->get(&health);
	};
	AttributeType getType(){return ATTRIBUTE_HEALTH;}
	std::string getName(){return "Health";}
};

struct DLL_U Attribute_Damage : public IAttribute
{
	Attribute_Damage();
	~Attribute_Damage();

	float damage;
	int owner_entityID;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add(damage,			"damage");
		list->add(owner_entityID,	"owner_entityID");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get(&damage);
		list->get(&owner_entityID);
	};
	AttributeType getType(){return ATTRIBUTE_DAMAGE;}
	std::string getName(){return "Damage";}
};

struct DLL_U Attribute_PlayerSpawnPoint : public IAttribute
{
	Attribute_PlayerSpawnPoint();
	~Attribute_PlayerSpawnPoint();

	AttributePtr<Attribute_Position> ptr_position;

	float secondsSinceLastSpawn;	//!< Is reset when a player spawns at the spawn point.
	float spawnArea;				//!< Defines the spawn point zone, a horizontal circle area.

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add_AttributePointer(ptr_position.index, "ptr_position");
		list->add(secondsSinceLastSpawn,	"secondsSinceLastSpawn");
		list->add(spawnArea,			"spawnArea");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		list->get(&secondsSinceLastSpawn);
		list->get(&spawnArea);
	};
	AttributeType getType(){return ATTRIBUTE_PLAYERSPAWNPOINT;}
	std::string getName(){return "PlayerSpawnPoint";}
};

enum PickupableType;
struct DLL_U Attribute_PickupablesSpawnPoint : public IAttribute
{
	Attribute_PickupablesSpawnPoint();
	~Attribute_PickupablesSpawnPoint();

	AttributePtr<Attribute_Position> ptr_position;

	PickupableType spawnPickupableType;			//!< Type of pickupable spawned by this pickupables spawn point
	float spawnDelayInSeconds;					//!< Delay until a pickupable may spawn
	float secondsSinceLastSpawn;				//!< Incrementing timer, reset when spawned.
	float secondsSinceLastPickup;				//!< Incrementing timer, reset when picked up.
	int maxNrOfExistingSpawnedPickupables;		//!< Is checked against "currentNrOfExistingSpawnedPickupables"
	int currentNrOfExistingSpawnedPickupables;	//!< Incremented when a pickubalbe is spawned from this pickupables spawn point. Decremented when a pickupable is picked up

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add_AttributePointer(ptr_position.index, "ptr_position");
		//list->add(spawnPickupableType, "spawnPickupableType");
		list->add(spawnDelayInSeconds, "spawnDelayInSeconds");
		list->add(secondsSinceLastSpawn, "secondsSinceLastSpawn");
		list->add(secondsSinceLastPickup, "secondsSinceLastPickup");
		list->add(maxNrOfExistingSpawnedPickupables, "maxNrOfExistingSpawnedPickupables");
		list->add(currentNrOfExistingSpawnedPickupables, "currentNrOfExistingSpawnedPickupables");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		//list->get(&spawnPickupableType);
		list->get(&spawnDelayInSeconds);
		list->get(&secondsSinceLastSpawn);
		list->get(&secondsSinceLastPickup);
		list->get(&maxNrOfExistingSpawnedPickupables);
		list->get(&currentNrOfExistingSpawnedPickupables);
	};

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

	PickupableType pickupableType;						//! MEDKIT, AMMUNITION_BULLET, AMMUNITION_SCATTER, AMMUNITION_EXPLOSIVE, etc
	int amount;											//! Data of pickupable (health, ammo, etc)

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add_AttributePointer(ptr_position.index, "ptr_position");
		list->add_AttributePointer(ptr_physics.index, "ptr_physics");
		list->add_AttributePointer(ptr_pickupablesSpawnPoint_creator.index, "ptr_creatorPickupablesSpawnPoint");
		//list->add_AttributePointer(pickupableType, "pickupableType");
		list->add(amount, "amount");

		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_position.index);
		list->get_AttributePointer(&ptr_physics.index);
		list->get_AttributePointer(&ptr_pickupablesSpawnPoint_creator.index);
		//list->get_AttributePointer(&pickupableType);
		list->get(&amount);
	};

	AttributeType getType(){return ATTRIBUTE_PICKUPABLE;}
	std::string getName(){return "Pickupable";}
};

class MutatorSettings;
#include "WeaponStructs.h"
/// Stores everything needed for the weapon system. The two enums "AmmunitionType" and "FiringMode" is used to preset the weapon settings. These settings are used in GameComponent to simulate the weapon behavior of choice.
/** 
\ingroup ATTRIBUTES
*/
struct DLL_U Attribute_WeaponStats : public IAttribute
{
	Attribute_WeaponStats();
	~Attribute_WeaponStats();

	Ammunition ammunition[Ammunition::NROFAMMUNITIONTYPES];
	FiringMode firingMode[FiringMode::NROFFIRINGMODETYPES];

	Ammunition::AmmunitionType currentAmmunitionType;
	FiringMode::FiringModeType currentFiringModeType;

	void setWeaponStats(Ammunition::AmmunitionType ammunitionType, FiringMode::FiringModeType firingModeType);

	std::string getAmmunitionTypeAsString();
	std::string getFiringModeAsString();

	DataItemList* getDataList();
	void saveTo(DataItemList* list);
	AttributeType getType(){return ATTRIBUTE_WEAPONSTATS;}
	std::string getName(){return "WeaponStats";}
};

struct DebugShape;
struct DLL_U Attribute_DebugShape : public IAttribute
{
	Attribute_DebugShape();
	~Attribute_DebugShape();
	void clean();

	
	AttributePtr<Attribute_Spatial> ptr_spatial;

	unsigned int	meshID;		//!< ID of mesh
	DebugShape*		shape;
	bool			render;

	DataItemList* getDataList();
	void saveTo(DataItemList* list);;
	AttributeType getType(){return ATTRIBUTE_WEAPONSTATS;}
	std::string getName(){return "WeaponStats";}
};

struct DLL_U Attribute_ExplosionSphere : public IAttribute
{
	Attribute_ExplosionSphere();
	~Attribute_ExplosionSphere();

	AttributePtr<Attribute_Physics> ptr_physics;
	float currentLifeTimeLeft;
	float radius;

	DataItemList* getDataList()
	{
		DataItemList* list = new DataItemList();
		list->add_AttributePointer(ptr_physics.index, "ptr_physics");;
		list->add(currentLifeTimeLeft,	"currentLifeTimeLeft");
		list->add(radius, "radius");
		return list;
	}
	void saveTo(DataItemList* list)
	{
		list->get_AttributePointer(&ptr_physics.index);
		list->get(&currentLifeTimeLeft);
		list->get(&radius);
	};
	AttributeType getType(){return ATTRIBUTE_EXPLOSIONSPHERE;}
	std::string getName(){return "ExplosionSphere";}
};