#pragma once

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
	AttributeType* AttributeName = AttributeName = AttributeManager::getInstance()->AttributeName##Attributes_.createAttribute(OwnerEntity)
	
	// Connects the AttributePointer by the name PointerName inside AttributeName with a AttributePointer created inside AttributeManager.
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
		position->position.x = 0.0f;
		position->position.y = 0.0f;
		position->position.z = 0.0f;

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->scale.x = 1.0f;
		spatial->scale.y = 1.0f;
		spatial->scale.z = 1.0f;

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshIndex = 0;

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);

		CREATE_ATTRIBUTE(InputAttribute, input, entity);
		CONNECT_ATTRIBUTES(input, physics);

		CREATE_ATTRIBUTE(CameraAttribute, camera, entity);
		CONNECT_ATTRIBUTES(camera, spatial);

		CREATE_ATTRIBUTE(PlayerAttribute, player, entity);
		CONNECT_ATTRIBUTES(player, render);
		CONNECT_ATTRIBUTES(player, input);

		static int playerId = 0;
		player->name = "Printer Terror";
		player->id = playerId;
		playerId++;
	}

	void createWorldEntity(Entity* entity)
	{
		
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);
		position->position.x = 0.0f;
		position->position.y = 0.0f;
		position->position.z = 0.0f;

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->scale.x = 1.0f;
		spatial->scale.y = 1.0f;
		spatial->scale.z = 1.0f;

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);
		render->meshIndex = 1;

		//CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		//CONNECT_ATTRIBUTES(physics, spatial);
	}

	void createProjectileEntity(Entity* entity, Event_createProjectile* e)
	{
		CREATE_ATTRIBUTE(PositionAttribute, position, entity);
		position->position.x = e->position.x;
		position->position.y = e->position.y;
		position->position.z = e->position.z;

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);
		spatial->rotation.x = e->direction.x;
		spatial->rotation.y = e->direction.y;
		spatial->rotation.z = e->direction.z;
		spatial->rotation.w = e->direction.w;

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);

		CREATE_ATTRIBUTE(PhysicsAttribute, physics, entity);
		CONNECT_ATTRIBUTES(physics, spatial);
		physics->isProjectile = true;
		physics->linearVelocity.y = 1.0f;
	}

	void createMesh(Entity* entity, Event_createMesh* e)
	{
		MeshAttribute* meshAttribute = AttributeManager::getInstance()->meshAttributes_.createAttribute(entity);
		meshAttribute->mesh		= e->mesh;
		meshAttribute->dynamic	= e->dynamic;
	}
};

	//
	// Undefine evil macros
	//

#undef CREATE_ATTRIBUTE
#undef CONNECT_ATTRIBUTES
