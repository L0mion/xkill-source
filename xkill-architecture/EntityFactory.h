#pragma once

//#include <xkill-utilities/DebugShape.h>
#include <xkill-utilities/Entity.h>
#include <xkill-utilities/AttributeManager.h>

// Iterators
ATTRIBUTES_DECLARE_ALL;

/// A factory for creating Entities and assigning multiple \ref ATTRIBUTES in a flexible way.
/** 
Filling out each Attribute and connecting chains of 
Attribute is the responsibility of the EntityFactory.

The AttributeFactory can be used to facilitate creation of \ref ATTRIBUTES.

\ingroup ARCHITECTURE
*/




class EntityFactory
{
public:
	// Creates an AttributeType (e.g. PositionAttribute) with name AttributeName (e.g. position) owned by Entity OwnerEntity.
	// IMPORTANT: AttributeName (e.g. position) is used to access attributes from AttributeManager (e.g. positionAttributes_).
	// if a longer name is used, such as positionAttribute, it will "copy paste" the name when accessing AttributeManager.
	// An AttributeName such as "positionAttribute" will result in accessing "positionAttributeAttributes_" inside
	// AttributeManager instead of "'positionAttributes_" which will result in error. As long as a shorter naming convention
	// such as "position" is used, this will not be a problem.
#define CREATE_ATTRIBUTE(POINTER_NAME, ATTRIBUTE_NAME, STORAGE_NAME, OWNER_ENTITY)						\
	AttributePtr<ATTRIBUTE_NAME> POINTER_NAME = AttributeManager::instance()->STORAGE_NAME.createAttribute(OWNER_ENTITY);

	enum ModelId
	{
			MODEL_PLACEHOLDER			= 0,

			MODEL_RIFLE					= 2,
			MODEL_HEALTHPACK			= 3,
			MODEL_AMMO_SINGLE			= 4,
			MODEL_AMMO_SCATTER			= 5,
			MODEL_AMMO_EXPLOSIVE		= 6,
			MODEL_PROCESS				= 7,
			MODEL_PROJECTILE_EXPLOSIVE	= 8,
			MODEL_PROJECTILE_SCATTER	= 9,
			MODEL_PROJECTILE_SINGLE		= 10,

			MODEL_LAST
	};

	EntityFactory()
	{
		ATTRIBUTES_INIT_ALL;
	}

	//! A player entity has the following attributes: position attribute, spatial attribute, render attribute, physics attribute, input attribute, camera attribute and player attribute
	//! Bindings:
	//! camera attribute --> spatial attribute --> position attribute
	//! player attribute --> render attribute --> spatial attribute --> position attribute
	//! Note: the player has the same spatial attribute as the camera.
	void createPlayerEntity(Entity* entity)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		
		CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
		ptr_render->ptr_spatial = ptr_spatial;
		
		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->meshID = ptr_render->meshID;
		ptr_physics->collisionFilterGroup = Attribute_Physics::PLAYER;
		ptr_physics->collisionFilterMask = Attribute_Physics::EVERYTHING;
		ptr_physics->gravity = Float3(0.0f, -0.0f, 0.0f);
		
		CREATE_ATTRIBUTE(ptr_input, Attribute_Input, input, entity);
		ptr_input->ptr_physics = ptr_physics;
		
		CREATE_ATTRIBUTE(ptr_health, Attribute_Health, health, entity);
		ptr_health->maxHealth = 100;
		
		CREATE_ATTRIBUTE(ptr_weaponStats, Attribute_WeaponStats, weaponStats, entity);
		ptr_weaponStats->currentAmmunitionType = XKILL_Enums::AmmunitionType::SCATTER;
		ptr_weaponStats->currentFiringModeType = XKILL_Enums::FiringModeType::AUTO;
		
		// Create camera
		AttributePtr<Attribute_Camera> ptr_camera = createCamera(entity, ptr_spatial); 
		
		CREATE_ATTRIBUTE(ptr_player, Attribute_Player, player, entity);
		ptr_player->ptr_render = ptr_render;
		ptr_player->ptr_input = ptr_input;
		ptr_player->ptr_camera = ptr_camera;
		ptr_player->ptr_health = ptr_health;
		ptr_player->ptr_weaponStats = ptr_weaponStats;
		ptr_player->meshID_whenAlive = MODEL_PROCESS;
		ptr_player->meshID_whenDead = MODEL_PROJECTILE_SCATTER;
		ptr_render->meshID = ptr_player->meshID_whenAlive;
		
		CREATE_ATTRIBUTE(ptr_splitScreen, Attribute_SplitScreen, splitScreen, entity);
		ptr_splitScreen->ptr_camera = ptr_camera;
		ptr_splitScreen->ptr_player = ptr_player;

		// Extra bindings
		ptr_physics->meshID = ptr_render->meshID;
		
		// Attach weapon
		AttributePtr<Attribute_Spatial> ptr_weaponFireLocation_spatial = createWeapon(entity, ptr_spatial, ptr_camera->ptr_spatial);
		ptr_player->ptr_weaponFireLocation_spatial = ptr_weaponFireLocation_spatial;
	}

	AttributePtr<Attribute_Camera> createCamera(Entity* entity, AttributePtr<Attribute_Spatial> ptr_parent_spatial)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		CREATE_ATTRIBUTE(ptr_camera, Attribute_Camera, camera, entity);
		ptr_camera->ptr_spatial = ptr_spatial;

		// Add behavior
		CREATE_ATTRIBUTE(ptr_offset, Behavior_Offset, offset, entity);
		ptr_offset->ptr_spatial = ptr_spatial;
		ptr_offset->ptr_parent_spatial_position = ptr_parent_spatial;
		ptr_offset->offset_position = Float3(0.0f, 0.6f, 0.16f);

		// Return
		return ptr_camera;
	}

	 AttributePtr<Attribute_Spatial> createWeapon(Entity* entity, AttributePtr<Attribute_Spatial> ptr_parent_spatial_position, AttributePtr<Attribute_Spatial> ptr_parent_spatial_rotation)
	{
		//
		// Create weapon
		//
		
		AttributePtr<Attribute_Spatial> ptr_weapon_spatial;
		{
			CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
			CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
			ptr_spatial->ptr_position = ptr_position;
			CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
			ptr_render->ptr_spatial = ptr_spatial;
			ptr_render->meshID = MODEL_RIFLE;
			CREATE_ATTRIBUTE(ptr_offset, Behavior_Offset, offset, entity);
			ptr_offset->ptr_spatial = ptr_spatial;
			ptr_offset->ptr_parent_spatial_position = ptr_parent_spatial_position;
			ptr_offset->ptr_parent_spatial_rotation = ptr_parent_spatial_rotation;
			ptr_offset->offset_position = Float3(0.23f, 0.4f, 0.56f);

			ptr_weapon_spatial = ptr_spatial;
		}

		
		//
		// Create fire location
		//

		AttributePtr<Attribute_Spatial> ptr_fireLocation_spatial;
		{
			CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
			CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
			ptr_spatial->ptr_position = ptr_position;
			/*CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
			ptr_render->ptr_spatial = ptr_spatial;
			ptr_render->meshID = MODEL_HEALTHPACK;*/
			CREATE_ATTRIBUTE(ptr_offset, Behavior_Offset, offset, entity);
			ptr_offset->ptr_spatial = ptr_spatial;
			ptr_offset->ptr_parent_spatial_position = ptr_weapon_spatial;
			ptr_offset->ptr_parent_spatial_rotation = ptr_weapon_spatial;
			ptr_offset->offset_position = Float3(0.0f, 0.06f, 0.32f);

			ptr_fireLocation_spatial = ptr_spatial;
		}

		return ptr_fireLocation_spatial;
	}
	
	void createWorldEntity(Entity* entity, Event_CreateWorld* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->position;
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		ptr_spatial->rotation = e->rotation;
		CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
		ptr_render->ptr_spatial = ptr_spatial;
		ptr_render->meshID = e->meshID;
		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->meshID = e->meshID;
		ptr_physics->collisionFilterGroup = Attribute_Physics::WORLD;
		ptr_physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::PROJECTILE | Attribute_Physics::FRUSTUM | Attribute_Physics::PICKUPABLE;
		ptr_physics->mass = 0;
	}

	void createProjectileEntity(Entity* entity, Event_CreateProjectile* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->position;

		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		ptr_spatial->rotation = e->rotation;

		//float lifeTime;
		CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
		ptr_render->ptr_spatial = ptr_spatial;
		ptr_render->meshID = MODEL_PROJECTILE_SINGLE;
		switch (e->ammunitionType)
		{
		case XKILL_Enums::AmmunitionType::BULLET:
			//lifeTime = 10.0f;
			ptr_render->meshID = MODEL_PROJECTILE_SINGLE;
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			//lifeTime = 10.0f;
			ptr_render->meshID = MODEL_PROJECTILE_EXPLOSIVE;
			break;
		case XKILL_Enums::AmmunitionType::SCATTER:
			//lifeTime = 0.1f;
			ptr_render->meshID = MODEL_PROJECTILE_SCATTER;
			break;
		default:
			break;
		}

		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->collisionFilterGroup = Attribute_Physics::PROJECTILE;
		ptr_physics->collisionFilterMask = Attribute_Physics::WORLD | Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::PICKUPABLE;
		ptr_physics->meshID = ptr_render->meshID;
		ptr_physics->linearVelocity = e->velocity;
		ptr_physics->mass = 50.0f;
		ptr_physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		ptr_physics->collisionResponse = true;

		CREATE_ATTRIBUTE(ptr_projectile, Attribute_Projectile, projectile, entity);
		ptr_projectile->ptr_physics = ptr_physics;
		ptr_projectile->entityIdOfCreator = e->entityIdOfCreator;
		ptr_projectile->ammunitionType = e->ammunitionType;
		ptr_projectile->firingModeType = e->firingMode;
		ptr_projectile->currentLifeTimeLeft = 10.0f;

		CREATE_ATTRIBUTE(ptr_damage, Attribute_Damage, damage, entity);
		ptr_damage->damage = e->damage;
		ptr_damage->owner_entityID = e->entityIdOfCreator;

		//temp, create demo light for each projectile
		CREATE_ATTRIBUTE(ptr_lightPoint, Attribute_Light_Point, lightPoint, entity);
		ptr_lightPoint->ptr_position			= ptr_position;

		Float4 color;
		switch (e->ammunitionType)
		{
		case XKILL_Enums::AmmunitionType::BULLET:
			color = Float4(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case XKILL_Enums::AmmunitionType::SCATTER:
			color = Float4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}

		ptr_lightPoint->lightPoint.ambient		= Float4(0.0f, 0.0f, 0.0f, 1.0f);
		ptr_lightPoint->lightPoint.diffuse		= color;
		ptr_lightPoint->lightPoint.specular		= color;
		ptr_lightPoint->lightPoint.range		= 1.0f;
		ptr_lightPoint->lightPoint.attenuation	= Float3(0.0f, 30.0f, 0.0f);
	}

	void createMesh(Entity* entity, Event_CreateMesh* e)
	{
		CREATE_ATTRIBUTE(ptr_mesh, Attribute_Mesh, mesh, entity);
		ptr_mesh->mesh		= e->mesh;
		ptr_mesh->dynamic	= e->dynamic;
		ptr_mesh->meshID	= e->id;
		ptr_mesh->fileName	= e->fileName;
		ptr_mesh->vertexType = e->vertexType;
	}

	void createPlayerSpawnPointEntity(Entity* entity, Event_CreatePlayerSpawnPoint* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->spawnPointPosition;
		CREATE_ATTRIBUTE(ptr_playerSpawnPoint, Attribute_PlayerSpawnPoint, playerSpawnPoint, entity);
		ptr_playerSpawnPoint->ptr_position = ptr_position;
		ptr_playerSpawnPoint->secondsSinceLastSpawn = 0.1f;
		ptr_playerSpawnPoint->spawnArea = e->spawnAreaRadius;
	}

	void createPickupablesSpawnPointEntity(Entity* entity, Event_CreatePickupablesSpawnPoint* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->spawnPointPosition;
		CREATE_ATTRIBUTE(ptr_pickupablesSpawnPoint, Attribute_PickupablesSpawnPoint, pickupablesSpawnPoint, entity);
		ptr_pickupablesSpawnPoint->ptr_position = ptr_position;
		ptr_pickupablesSpawnPoint->spawnPickupableType = e->pickupableType;
		ptr_pickupablesSpawnPoint->spawnDelayInSeconds = 5.0f;
		ptr_pickupablesSpawnPoint->maxNrOfExistingSpawnedPickupables = 1;
	}

	void createPickupableEntity(Entity* entity, Event_CreatePickupable* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->position;
		
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		
		CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
		ptr_render->ptr_spatial = ptr_spatial;
		switch (e->pickupableType)
		{
		case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
			ptr_render->meshID = MODEL_AMMO_SINGLE;
			break;
		case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
			ptr_render->meshID = MODEL_AMMO_SCATTER;
			break;
		case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
			ptr_render->meshID = MODEL_AMMO_EXPLOSIVE;
			break;
		case XKILL_Enums::PickupableType::MEDKIT:
			ptr_render->meshID = MODEL_HEALTHPACK;
		default:
			break;
		}

		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->collisionFilterGroup = Attribute_Physics::PICKUPABLE;
		ptr_physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::WORLD | Attribute_Physics::PICKUPABLE | Attribute_Physics::PROJECTILE | Attribute_Physics::EXPLOSIONSPHERE;
		ptr_physics->collisionResponse = true;
		ptr_physics->mass = 10.0f;
		ptr_physics->gravity = Float3(0.0f, -10.0f, 0.0f);
		ptr_physics->meshID = ptr_render->meshID;

		CREATE_ATTRIBUTE(ptr_pickupable, Attribute_Pickupable, pickupable, entity);
		ptr_pickupable->ptr_position = ptr_position;
		ptr_pickupable->ptr_physics = ptr_physics;
		ptr_pickupable->ptr_pickupablesSpawnPoint_creator = e->creatorPickupablesSpawnPoint;
		ptr_pickupable->amount = e->amount;
		ptr_pickupable->pickupableType = e->pickupableType;

		AttributePtr<Attribute_PickupablesSpawnPoint> pickupablesSpawnPoint = e->creatorPickupablesSpawnPoint;

		pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables++;
	}

	void createExplosionSphereEntity(Entity* entity, Event_CreateExplosionSphere* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->position;

		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;

		/*
		CREATE_ATTRIBUTE(ptr_debugShape, Attribute_DebugShape, debugShape, entity);
		ptr_debugShape->ptr_spatial = ptr_spatial;
		ptr_debugShape->shape	= new DebugShapeSphere(e->radius);
		ptr_debugShape->render	= true;
		*/

		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->collisionFilterGroup = Attribute_Physics::EXPLOSIONSPHERE;
		ptr_physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::PICKUPABLE;
		ptr_physics->collisionResponse = true;
		ptr_physics->mass = 0.0f;
		ptr_physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		ptr_physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);

		CREATE_ATTRIBUTE(ptr_explosionSphere, Attribute_ExplosionSphere, explosionSphere, entity);
		ptr_explosionSphere->ptr_physics = ptr_physics;
		ptr_explosionSphere->ammunitionType = e->ammunitionType;
		ptr_explosionSphere->firingModeType = e->firingModeType;

		CREATE_ATTRIBUTE(ptr_damage, Attribute_Damage, damage, entity);
		ptr_damage->damage = e->damage;
		ptr_damage->owner_entityID = e->entityIdOfCreator;
	}

	void createLightEntity(Entity* entity, Event_CreateLight* e)
	{
		// TEMP: Create demo light for each projectile
		if(e->type == 1)
		{
			CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
			ptr_position->position = e->position;
			CREATE_ATTRIBUTE(ptr_lightPoint, Attribute_Light_Point, lightPoint, entity);
			ptr_lightPoint->ptr_position = ptr_position;
			ptr_lightPoint->lightPoint.ambient = Float4(e->ambient,1);
			ptr_lightPoint->lightPoint.diffuse = Float4(e->diffuse,1);
			ptr_lightPoint->lightPoint.specular = Float4(e->specular,1);
			ptr_lightPoint->lightPoint.attenuation = e->attenuation;
			ptr_lightPoint->lightPoint.range = e->range;
		}
		else if(e->type == 2)
		{
			CREATE_ATTRIBUTE(ptr_lightDir, Attribute_Light_Dir, lightDir, entity);
			ptr_lightDir->lightDir.ambient = Float4(e->ambient,1);
			ptr_lightDir->lightDir.diffuse = Float4(e->diffuse,1);
			ptr_lightDir->lightDir.specular = Float4(e->specular,1);
			ptr_lightDir->lightDir.direction = e->direction.normalize();
		}
		else if(e->type == 3)
		{
			CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
			ptr_position->position = e->position;
			CREATE_ATTRIBUTE(ptr_lightSpot, Attribute_Light_Spot, lightSpot, entity);
			ptr_lightSpot->ptr_position = ptr_position;
			ptr_lightSpot->lightSpot.ambient = Float4(e->ambient,1);
			ptr_lightSpot->lightSpot.diffuse = Float4(e->diffuse,1);
			ptr_lightSpot->lightSpot.specular = Float4(e->specular,1);
			ptr_lightSpot->lightSpot.attenuation = e->attenuation;
			ptr_lightSpot->lightSpot.direction = e->direction;
			ptr_lightSpot->lightSpot.range = e->range;
			ptr_lightSpot->lightSpot.spotPow = e->spotPow;
		}
	}


	void createInputDevice(Entity* entity, Event_CreateInputDevice* e)
	{
		CREATE_ATTRIBUTE(ptr_inputDevice, Attribute_InputDevice, inputDevice, entity);
		ptr_inputDevice->device = e->device;
	}
};

	//
	// Undefine evil macros
	//

#undef CREATE_ATTRIBUTE
//#undef CONNECT_ATTRIBUTES
