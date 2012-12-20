#pragma once

#include <xkill-utilities/DebugShape.h>

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"

#include <vector>
#include <iostream>


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
	AttributeType* AttributeName = AttributeManager::getInstance()->AttributeName##Attributes_.createAttribute(OwnerEntity)
	
	// Connects the AttributePointer by the name PointerName inside AttributeName with latest AttributePointer created inside AttributeManager.
	// IMPORTANT: The following formula is used to access AttributeManager, "PointerName+Attributes".
	// PointerName "position" will result in "positionAttributes" which will work.
	// PointerName "positionAttribute" will result in "positionAttributeAttributes" which will fail.
#define CONNECT_ATTRIBUTES(AttributeName, PointerName)									\
	AttributeName->PointerName##Attribute = AttributeManager::getInstance()->PointerName##Attributes_.getLatestAttributeAsAttributePointer()

	//! A player entity has the following attributes: position attribute, spatial attribute, render attribute, physics attribute, input attribute, camera attribute and player attribute
	//! Bindings:
	//! camera attribute --> spatial attribute --> position attribute
	//! player attribute --> render attribute --> spatial attribute --> position attribute
	//! Note: the player has the same spatial attribute as the camera.
	void createPlayerEntity(Entity* entity)
	{
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = 0;

		CREATE_ATTRIBUTE(DebugShapeAttribute, debugShape, entity);	//create temp debug shape
		CONNECT_ATTRIBUTES(debugShape, spatial);
		debugShape->meshID = render->meshID;
		debugShape->shape	=  nullptr;/*new DebugShapeBB(
			Float3(-0.5f, -0.5f, -0.5f),
			Float3(0.5f, 0.5f, 0.5f)); //new DebugShapeSphere(1.0f);*/
		debugShape->render	= false;

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = render->meshID;
		
		CREATE_ATTRIBUTE(InputAttribute, input, entity);
		CONNECT_ATTRIBUTES(input, physics);
		input->changeAmmunitionType = false;
		input->changeFiringMode = false;

		CREATE_ATTRIBUTE(CameraAttribute, camera, entity);
		CONNECT_ATTRIBUTES(camera, spatial);

		CREATE_ATTRIBUTE(HealthAttribute, health, entity);

		CREATE_ATTRIBUTE(WeaponStatsAttribute, weaponStats, entity);

		CREATE_ATTRIBUTE(PlayerAttribute, player, entity);
		CONNECT_ATTRIBUTES(player, render);
		CONNECT_ATTRIBUTES(player, input);
		CONNECT_ATTRIBUTES(player, camera);
		CONNECT_ATTRIBUTES(player, health);
		CONNECT_ATTRIBUTES(player, weaponStats);
		//player->name = "Process Name";
		static int playerId = 0;
		player->id = playerId;
		playerId++;
	}
	
	void createWorldEntity(Entity* entity)
	{
		static int HACKHACK = 1;
		
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		
		render->meshID = HACKHACK;

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = render->meshID;
		
		physics->mass = 0;
				
		HACKHACK+=2;
	}

	void createProjectileEntity(Entity* entity, Event_CreateProjectile* e)
	{
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->rotation = e->rotation;

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshID = 2;

		CREATE_ATTRIBUTE(DebugShapeAttribute, debugShape, entity);	//create temp debug shape
		CONNECT_ATTRIBUTES(debugShape, spatial);
		debugShape->meshID = render->meshID;
		debugShape->shape	=  nullptr;/*new DebugShapeBB(
			Float3(-0.5f, -0.5f, -0.5f),
			Float3(0.5f, 0.5f, 0.5f)); //new DebugShapeSphere(1.0f);*/
		debugShape->render	= false;

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		CONNECT_ATTRIBUTES(physics, render);
		physics->meshID = render->meshID;
		
		physics->isProjectile = true;
		physics->linearVelocity = e->velocity;
		physics->mass = 100.0f;
		physics->gravity = Float3(0.0f, 0.0f, 0.0f);
		physics->collisionResponse = true;

		CREATE_ATTRIBUTE(ProjectileAttribute, projectile, entity);
		CONNECT_ATTRIBUTES(projectile, physics);
		projectile->entityIdOfCreator = e->entityIdOfCreator;
		projectile->explodeOnImnpact = e->explodeOnImpact;

		CREATE_ATTRIBUTE(DamageAttribute, damage, entity);
		damage->damage = e->damage;
		damage->owner_entityID = e->entityIdOfCreator;
	}

	void createMesh(Entity* entity, Event_CreateMesh* e)
	{
		MeshAttribute* meshAttribute = AttributeManager::getInstance()->meshAttributes_.createAttribute(entity);
		meshAttribute->mesh		= e->mesh;
		meshAttribute->dynamic	= e->dynamic;
		meshAttribute->meshID	= e->id;
	}

	void createSpawnPointEntity(Entity* entity, Event_CreateSpawnPoint* e)
	{
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);
		position->position = e->spawnPointPosition;
		
		CREATE_ATTRIBUTE(SpawnPointAttribute, spawnPoint, entity);
		CONNECT_ATTRIBUTES(spawnPoint, position);
		spawnPoint->timeSinceLastSpawn = 0.1f;
		spawnPoint->spawnArea = e->spawnAreaRadius;
	}

	void createExplosionSphere(Entity* entity, Event_CreateExplosionSphere* e)
	{
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);
		position->position = e->position;

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(DebugShapeAttribute, debugShape, entity);	//create temp debug shape
		CONNECT_ATTRIBUTES(debugShape, spatial);
		debugShape->shape	= new DebugShapeSphere(e->radius*100.0f);
		debugShape->render	= true;

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		physics->isExplosionSphere = true;
		physics->explosionSphereRadius = e->radius;
		physics->collisionResponse = false;
		physics->mass = 0.0f;
		physics->gravity = Float3(0.0f, 0.0f, 0.0f);

		CREATE_ATTRIBUTE(ExplosionSphereAttribute, explosionSphere, entity);
		CONNECT_ATTRIBUTES(explosionSphere, physics);

		CREATE_ATTRIBUTE(DamageAttribute, damage, entity);
		damage->damage = e->damage;
		damage->owner_entityID = e->entityIdOfCreator;
	}
};

	//
	// Undefine evil macros
	//

#undef CREATE_ATTRIBUTE
#undef CONNECT_ATTRIBUTES
