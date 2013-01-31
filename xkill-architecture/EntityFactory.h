#pragma once

#include <xkill-utilities/DebugShape.h>
#include <xkill-utilities/Entity.h>
#include <xkill-utilities/AttributeManager.h>
#include "EntityManager.h"

#include <vector>
#include <iostream>

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
	A_Ptr<ATTRIBUTE_NAME> POINTER_NAME = AttributeManager::instance()->STORAGE_NAME.createAttribute(OWNER_ENTITY);

	// Connects the AttributePointer by the name PointerName inside AttributeName with latest AttributePointer created inside AttributeManager.
	// IMPORTANT: The following formula is used to access AttributeManager, "PointerName+Attributes".
	// PointerName "position" will result in "positionAttributes" which will work.
	// PointerName "positionAttribute" will result in "positionAttributeAttributes" which will fail.
//#define __CONNECT_ATTRIBUTES(POINTER_NAME, ATTRIBUTE_NAME, CONNECTING_POINTER_NAME)									\
//	POINTER_NAME.attribute->ATTRIBUTE_NAME = CONNECTING_POINTER_NAME;

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
		ptr_render->meshID = 0;
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
		ptr_health->startHealth = 100;
		CREATE_ATTRIBUTE(ptr_weaponStats, Attribute_WeaponStats, weaponStats, entity);
		ptr_weaponStats->currentAmmunitionType = Ammunition::SCATTER;
		ptr_weaponStats->currentFiringModeType = FiringMode::AUTO;
		// Create camera
		A_Ptr<Attribute_Camera> ptr_camera = createCamera(entity); 
		CREATE_ATTRIBUTE(ptr_player, Attribute_Player, player, entity);
		ptr_player->ptr_render = ptr_render;
		ptr_player->ptr_input = ptr_input;
		ptr_player->ptr_camera = ptr_camera;
		ptr_player->ptr_health = ptr_health;
		ptr_player->ptr_weaponStats = ptr_weaponStats;
		CREATE_ATTRIBUTE(ptr_splitScreen, Attribute_SplitScreen, splitScreen, entity);
		ptr_splitScreen->ptr_camera = ptr_camera;
		ptr_splitScreen->ptr_player = ptr_player;
	}

	A_Ptr<Attribute_Camera> createCamera(Entity* entity)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		CREATE_ATTRIBUTE(ptr_camera, Attribute_Camera, camera, entity);
		ptr_camera->ptr_spatial = ptr_spatial;

		// Add behavior
		CREATE_ATTRIBUTE(ptr_offset, Behavior_Offset, offset, entity);
		ptr_offset->offset_position = Float3(0.0f, 2.0f, 0.0f);

		// Return
		return ptr_camera;
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
		CREATE_ATTRIBUTE(ptr_spatial,Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		ptr_spatial->rotation = e->rotation;
		CREATE_ATTRIBUTE(ptr_render, Attribute_Render, render, entity);
		ptr_render->ptr_spatial = ptr_spatial;
		ptr_render->meshID = 2;
		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->collisionFilterGroup = Attribute_Physics::PROJECTILE;
		ptr_physics->collisionFilterMask = Attribute_Physics::WORLD | Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::PICKUPABLE;
		ptr_physics->meshID = ptr_render->meshID;
		ptr_physics->linearVelocity = e->velocity;
		ptr_physics->mass = 100.0f;
		ptr_physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		ptr_physics->collisionResponse = true;
		CREATE_ATTRIBUTE(ptr_projectile, Attribute_Projectile, projectile, entity);
		ptr_projectile->ptr_physics = ptr_physics;
		ptr_projectile->entityIdOfCreator = e->entityIdOfCreator;
		ptr_projectile->explodeOnImnpact = e->explodeOnImpact;
		ptr_projectile->explosionSphereRadius = e->explosionSphereRadius;
		CREATE_ATTRIBUTE(ptr_damage, Attribute_Damage, damage, entity);
		ptr_damage->damage = e->damage;
		ptr_damage->owner_entityID = e->entityIdOfCreator;
		//temp, create demo light for each projectile
		CREATE_ATTRIBUTE(ptr_lightPoint, Attribute_Light_Point, lightPoint, entity);
		ptr_lightPoint->ptr_position			= ptr_position;
		ptr_lightPoint->lightPoint.ambient		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		ptr_lightPoint->lightPoint.diffuse		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		ptr_lightPoint->lightPoint.specular		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		ptr_lightPoint->lightPoint.range		= 1.0f;
		ptr_lightPoint->lightPoint.attenuation	= Float3(0.0f, 0.1f, 0.0f);
	}

	void createMesh(Entity* entity, Event_CreateMesh* e)
	{
		CREATE_ATTRIBUTE(ptr_mesh, Attribute_Mesh, mesh, entity);
		ptr_mesh->mesh		= e->mesh;
		ptr_mesh->dynamic	= e->dynamic;
		ptr_mesh->meshID	= e->id;
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
		ptr_pickupablesSpawnPoint->spawnDelayInSeconds = 0.1f;
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
		/*
		switch (e->pickupableType)
		{
		case PickupableType::AMMUNITION_BULLET:
			render->meshID = 4;
			break;
		case PickupableType::AMMUNITION_SCATTER:
			render->meshID = 5;
			break;
		case PickupableType::AMMUNITION_EXPLOSIVE:
			render->meshID = 6;
			break;
		case PickupableType::MEDKIT:
			render->meshID = 3;
		default:
			break;
		}
		*/
		ptr_render->meshID = 1;
		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->ptr_render = ptr_render;
		ptr_physics->collisionFilterGroup = Attribute_Physics::PICKUPABLE;
		ptr_physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::WORLD | Attribute_Physics::PICKUPABLE | Attribute_Physics::PROJECTILE;
		ptr_physics->collisionResponse = true;
		ptr_physics->mass = 10.0f;
		ptr_physics->gravity = Float3(0.0f, -10.0f, 0.0f);
		ptr_physics->meshID = ptr_render->meshID;
		CREATE_ATTRIBUTE(ptr_pickupable, Attribute_Pickupable, pickupable, entity);
		ptr_pickupable->ptr_position = ptr_position);
		ptr_pickupable->ptr_physics = ptr_physics;
		ptr_pickupable->ptr_pickupablesSpawnPoint_creator = e->creatorPickupablesSpawnPoint;
		ptr_pickupable->amount = e->amount;
		ptr_pickupable->pickupableType = e->pickupableType;

		// Increment parent
		Attribute_PickupablesSpawnPoint* pickupablesSpawnPoint = itrPickupablesSpawnPoint.at(e->creatorPickupablesSpawnPoint);
		pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables++;
	}

	void createExplosionSphereEntity(Entity* entity, Event_CreateExplosionSphere* e)
	{
		CREATE_ATTRIBUTE(ptr_position, Attribute_Position, position, entity);
		ptr_position->position = e->position;
		CREATE_ATTRIBUTE(ptr_spatial, Attribute_Spatial, spatial, entity);
		ptr_spatial->ptr_position = ptr_position;
		CREATE_ATTRIBUTE(ptr_debugShape, Attribute_DebugShape, debugShape, entity);
		ptr_debugShape->ptr_spatial = ptr_spatial;
		ptr_debugShape->shape	= new DebugShapeSphere(e->radius);
		ptr_debugShape->render	= true;
		CREATE_ATTRIBUTE(ptr_physics, Attribute_Physics, physics, entity);
		ptr_physics->ptr_spatial = ptr_spatial;
		ptr_physics->collisionFilterGroup = Attribute_Physics::EXPLOSIONSPHERE;
		ptr_physics->collisionFilterMask = Attribute_Physics::PLAYER;
		ptr_physics->collisionResponse = false;
		ptr_physics->mass = 0.0f;
		ptr_physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		ptr_physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
		CREATE_ATTRIBUTE(ptr_explosionSphere, Attribute_ExplosionSphere, explosionSphere, entity);
		ptr_explosionSphere->ptr_physics = ptr_physics;
		ptr_explosionSphere->radius = e->radius;
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
