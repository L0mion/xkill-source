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
#define CREATE_ATTRIBUTE(AttributeType, AttributeName, OwnerEntity)						\
	AttributeType* AttributeName = ((AttributeManager*)AttributeManagerDLLWrapper::getInstance())->AttributeName.createAttribute(OwnerEntity)

	// Connects the AttributePointer by the name PointerName inside AttributeName with latest AttributePointer created inside AttributeManager.
	// IMPORTANT: The following formula is used to access AttributeManager, "PointerName+Attributes".
	// PointerName "position" will result in "positionAttributes" which will work.
	// PointerName "positionAttribute" will result in "positionAttributeAttributes" which will fail.
#define CONNECT_ATTRIBUTES(AttributeName, PointerName)									\
	AttributeName->ptr_##PointerName = ((AttributeManager*)AttributeManagerDLLWrapper::getInstance())->PointerName.getLatestAttributeAsAttributePointer()

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
		ATTRIBUTES_INIT_ALL;

		CREATE_ATTRIBUTE(Attribute_Position, position, entity);

		CREATE_ATTRIBUTE(Attribute_Spatial, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(Attribute_Render, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = 0;

		//CREATE_ATTRIBUTE(Attribute_DebugShape, debugShape, entity);	//create temp debug shape
		//CONNECT_ATTRIBUTES(debugShape, spatial);
		//debugShape->meshID = render->meshID;
		//debugShape->shape	=  new DebugShapeSphere(1.0f);/*new DebugShapeBB(
		//	Float3(-0.5f, -0.5f, -0.5f),
		//	Float3(0.5f, 0.5f, 0.5f)); //new DebugShapeSphere(1.0f);*/
		//debugShape->render	= true;

		CREATE_ATTRIBUTE(Attribute_Physics, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = render->meshID;
		physics->collisionFilterGroup = Attribute_Physics::PLAYER;
		physics->collisionFilterMask = Attribute_Physics::EVERYTHING;
		
		CREATE_ATTRIBUTE(Attribute_Input, input, entity);
		CONNECT_ATTRIBUTES(input, physics);

		CREATE_ATTRIBUTE(Attribute_Camera, camera, entity);
		CONNECT_ATTRIBUTES(camera, spatial);

		CREATE_ATTRIBUTE(Attribute_Health, health, entity);
		health->startHealth = 100;

		CREATE_ATTRIBUTE(Attribute_WeaponStats, weaponStats, entity);
		weaponStats->currentAmmunitionType = Ammunition::SCATTER;
		weaponStats->currentFiringModeType = FiringMode::AUTO;

		CREATE_ATTRIBUTE(Attribute_Player, player, entity);
		CONNECT_ATTRIBUTES(player, render);
		CONNECT_ATTRIBUTES(player, input);
		CONNECT_ATTRIBUTES(player, camera);
		CONNECT_ATTRIBUTES(player, health);
		CONNECT_ATTRIBUTES(player, weaponStats);

		CREATE_ATTRIBUTE(Attribute_SplitScreen, splitScreen, entity);
		CONNECT_ATTRIBUTES(splitScreen, camera);
		CONNECT_ATTRIBUTES(splitScreen, player);
	}
	
	void createWorldEntity(Entity* entity, Event_CreateWorld* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(Attribute_Spatial, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->rotation = e->rotation;

		CREATE_ATTRIBUTE(Attribute_Render, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = e->meshID;

		CREATE_ATTRIBUTE(Attribute_Physics, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = e->meshID;
		physics->collisionFilterGroup = Attribute_Physics::WORLD;
		physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::PROJECTILE | Attribute_Physics::FRUSTUM | Attribute_Physics::PICKUPABLE;
		physics->mass = 0;

		position = ((AttributeManager*)AttributeManagerDLLWrapper::getInstance())->position.createAttribute(entity);
		position->position = Float3(0.0f, 0.5f, 0.0f);
		
		//CREATE_ATTRIBUTE(Attribute_Light_Dir, lightDir, entity);
		//lightDir->lightDir.direction	= Float3(0.57735f, -0.57735f, 0.57735f);
		//lightDir->lightDir.ambient		= Float4(0.2f, 0.2f, 0.2f, 1.0f);
		//lightDir->lightDir.diffuse		= Float4(0.2f, 0.2f, 0.2f, 1.0f);
		//lightDir->lightDir.specular		= Float4(0.2f, 0.2f, 0.2f, 1.0f);
	}

	void createProjectileEntity(Entity* entity, Event_CreateProjectile* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(Attribute_Spatial, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->rotation = e->rotation;

		CREATE_ATTRIBUTE(Attribute_Render, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = 2;

		//CREATE_ATTRIBUTE(Attribute_DebugShape, debugShape, entity);	//create temp debug shape
		//CONNECT_ATTRIBUTES(debugShape, spatial);
		//debugShape->meshID = render->meshID;
		//debugShape->shape	=  nullptr;/*new DebugShapeBB(
		//	Float3(-0.5f, -0.5f, -0.5f),
		//	Float3(0.5f, 0.5f, 0.5f)); //new DebugShapeSphere(1.0f);*/
		//debugShape->render	= false;

		CREATE_ATTRIBUTE(Attribute_Physics, physics, entity);
		physics->collisionFilterGroup = Attribute_Physics::PROJECTILE;
		physics->collisionFilterMask = Attribute_Physics::WORLD | Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::PICKUPABLE;
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = render->meshID;
		
		float scale = 100.0f;
		physics->linearVelocity = e->velocity; //Float3(e->velocity.x / scale,  e->velocity.y / scale, e->velocity.z / scale);
		physics->mass = 100.0f;
		physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		physics->collisionResponse = true;

		CREATE_ATTRIBUTE(Attribute_Projectile, projectile, entity);
		CONNECT_ATTRIBUTES(projectile, physics);
		projectile->entityIdOfCreator = e->entityIdOfCreator;
		projectile->explodeOnImnpact = e->explodeOnImpact;
		projectile->explosionSphereRadius = e->explosionSphereRadius;

		CREATE_ATTRIBUTE(Attribute_Damage, damage, entity);
		damage->damage = e->damage;
		damage->owner_entityID = e->entityIdOfCreator;

		//temp, create demo light for each projectile
		CREATE_ATTRIBUTE(Attribute_Light_Point, lightPoint, entity);
		CONNECT_ATTRIBUTES(lightPoint, position);
		lightPoint->lightPoint.ambient		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		lightPoint->lightPoint.diffuse		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		lightPoint->lightPoint.specular		= Float4(0.8f, 0.0f, 0.0f, 1.0f);
		lightPoint->lightPoint.range		= 1.0f;
		lightPoint->lightPoint.attenuation	= Float3(0.0f, 0.1f, 0.0f);
	}

	void createMesh(Entity* entity, Event_CreateMesh* e)
	{
		//MeshAttribute* meshAttribute = ATTRIBUTE_MANAGER->mesh.createAttribute(entity);
		CREATE_ATTRIBUTE(Attribute_Mesh, mesh, entity);
		mesh->mesh		= e->mesh;
		mesh->dynamic	= e->dynamic;
		mesh->meshID	= e->id;
		mesh->fileName	= e->fileName;
		mesh->vertexType = e->vertexType;
	}

	void createPlayerSpawnPointEntity(Entity* entity, Event_CreatePlayerSpawnPoint* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->spawnPointPosition;
		
		CREATE_ATTRIBUTE(Attribute_PlayerSpawnPoint, playerSpawnPoint, entity);
		CONNECT_ATTRIBUTES(playerSpawnPoint, position);
		playerSpawnPoint->secondsSinceLastSpawn = 0.1f;
		playerSpawnPoint->spawnArea = e->spawnAreaRadius;
	}

	void createPickupablesSpawnPointEntity(Entity* entity, Event_CreatePickupablesSpawnPoint* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->spawnPointPosition;

		CREATE_ATTRIBUTE(Attribute_PickupablesSpawnPoint, pickupablesSpawnPoint, entity);
		CONNECT_ATTRIBUTES(pickupablesSpawnPoint, position);
		pickupablesSpawnPoint->spawnPickupableType = e->pickupableType;
		pickupablesSpawnPoint->spawnDelayInSeconds = 0.1f;
		pickupablesSpawnPoint->maxNrOfExistingSpawnedPickupables = 1;
	}

	void createPickupableEntity(Entity* entity, Event_CreatePickupable* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(Attribute_Spatial, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(Attribute_Render, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = 1;

		CREATE_ATTRIBUTE(Attribute_Physics, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->collisionFilterGroup = Attribute_Physics::PICKUPABLE;
		physics->collisionFilterMask = Attribute_Physics::PLAYER | Attribute_Physics::FRUSTUM | Attribute_Physics::WORLD | Attribute_Physics::PICKUPABLE | Attribute_Physics::PROJECTILE;
		physics->collisionResponse = true;
		physics->mass = 10.0f;
		physics->gravity = Float3(0.0f, -10.0f, 0.0f);
		physics->meshID = render->meshID;

		CREATE_ATTRIBUTE(Attribute_Pickupable, pickupable, entity);
		pickupable->amount = e->amount;
		pickupable->pickupableType = e->pickupableType;
		pickupable->ptr_creatorPickupablesSpawnPoint = e->creatorPickupablesSpawnPoint;
		CONNECT_ATTRIBUTES(pickupable, position);
		CONNECT_ATTRIBUTES(pickupable, physics);

		Attribute_PickupablesSpawnPoint* pickupablesSpawnPoint = itrPickupablesSpawnPoint.at(e->creatorPickupablesSpawnPoint);
		pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables++;
	}

	void createExplosionSphereEntity(Entity* entity, Event_CreateExplosionSphere* e)
	{
		CREATE_ATTRIBUTE(Attribute_Position, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(Attribute_Spatial, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(Attribute_DebugShape, debugShape, entity);	//create temp debug shape
		CONNECT_ATTRIBUTES(debugShape, spatial);
		debugShape->shape	= new DebugShapeSphere(e->radius);
		debugShape->render	= true;

		CREATE_ATTRIBUTE(Attribute_Physics, physics, entity);
		physics->collisionFilterGroup = Attribute_Physics::EXPLOSIONSPHERE;
		physics->collisionFilterMask = Attribute_Physics::PLAYER;
		CONNECT_ATTRIBUTES(physics, spatial);
		physics->collisionResponse = false;
		physics->mass = 0.0f;
		physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);

		CREATE_ATTRIBUTE(Attribute_ExplosionSphere, explosionSphere, entity);
		CONNECT_ATTRIBUTES(explosionSphere, physics);
		explosionSphere->radius = e->radius;

		CREATE_ATTRIBUTE(Attribute_Damage, damage, entity);
		damage->damage = e->damage;
		damage->owner_entityID = e->entityIdOfCreator;
	}


	void createLightEntity(Entity* entity, Event_CreateLight* e)
	{
		//temp, create demo light for each projectile
		if(e->type == 1)
		{
			CREATE_ATTRIBUTE(Attribute_Position, position, entity);
			position->position = e->position;
			CREATE_ATTRIBUTE(Attribute_Light_Point, lightPoint, entity);
			CONNECT_ATTRIBUTES(lightPoint, position);
			lightPoint->lightPoint.ambient = Float4(e->ambient,1);
			lightPoint->lightPoint.diffuse = Float4(e->diffuse,1);
			lightPoint->lightPoint.specular = Float4(e->specular,1);
			lightPoint->lightPoint.attenuation = e->attenuation;
			lightPoint->lightPoint.range = e->range;
		}
		else if(e->type == 2)
		{
			CREATE_ATTRIBUTE(Attribute_Light_Dir, lightDir, entity);
			lightDir->lightDir.ambient = Float4(e->ambient,1);
			lightDir->lightDir.diffuse = Float4(e->diffuse,1);
			lightDir->lightDir.specular = Float4(e->specular,1);
			lightDir->lightDir.direction = e->direction.normalize();
		}
		else if(e->type == 3)
		{
			CREATE_ATTRIBUTE(Attribute_Position, position, entity);
			position->position = e->position;
			CREATE_ATTRIBUTE(Attribute_Light_Spot, lightSpot, entity);
			CONNECT_ATTRIBUTES(lightSpot, position);
			lightSpot->lightSpot.ambient = Float4(e->ambient,1);
			lightSpot->lightSpot.diffuse = Float4(e->diffuse,1);
			lightSpot->lightSpot.specular = Float4(e->specular,1);
			lightSpot->lightSpot.attenuation = e->attenuation;
			lightSpot->lightSpot.direction = e->direction;
			lightSpot->lightSpot.range = e->range;
			lightSpot->lightSpot.spotPow = e->spotPow;
		}
	}


	void createInputDevice(Entity* entity, Event_CreateInputDevice* e)
	{
		Attribute_InputDevice* inputDevice = itrInputDevice.createAttribute(entity);
		

		inputDevice->device = e->device;
	}
};

	//
	// Undefine evil macros
	//

#undef CREATE_ATTRIBUTE
#undef CONNECT_ATTRIBUTES
