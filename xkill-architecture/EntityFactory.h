#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"

#include <vector>
#include <iostream>

// Macro to create an Attribute
#define CREATE_ATTRIBUTE(AttributeType, AttributeName, OwnerEntity)									\
		AttributeType* AttributeName = AttributeName = AttributeManager::getInstance()->AttributeName##Attributes_.createAttribute(OwnerEntity);
// Macro to create connect the pointer of name PointerName inside AttributeName 
#define CONNECT_ATTRIBUTES(AttributeName, PointerName)												\
		AttributeName->PointerName##Attribute = AttributeManager::getInstance()->PointerName##Attributes_.getLatestAttributeAsAttributePointer();



/// A factory for creating Entities and assigning multiple \ref ATTRIBUTES in a flexible way.
/** 
Filling out each Attribute and connecting chains of 
Attribute is the responsibility of the EntityFactory.

The AttributeFactory can be used to facilitate creation of \ref ATTRIBUTES.

\ingroup ARCHITECTURE
*/


class EntityFactory 
{
private:
	/** 
	Creates an entity and assigns a unique ID
	*/
	Entity* createEntity()
	{
		static int id = 1;

		Entity* e = new Entity(id);
		id++;

		return e;
	}

public:

	//! A player entity has the following attributes: position attribute, spatial attribute, render attribute, physics attribute, input attribute, camera attribute and player attribute
	//! Bindings:
	//! camera attribute --> spatial attribute --> position attribute
	//! player attribute --> render attribute --> spatial attribute --> position attribute
	//! Note: the player has the same spatial attribute as the camera.
	Entity* createPlayerEntity()
	{
		Entity* entity = createEntity();

		CREATE_ATTRIBUTE(PositionAttribute, position, entity);

		CREATE_ATTRIBUTE(SpatialAttribute, spatial, entity);
		CONNECT_ATTRIBUTES(spatial, position);

		CREATE_ATTRIBUTE(RenderAttribute, render, entity);
		CONNECT_ATTRIBUTES(render, spatial);

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


		// Return entity
		return entity;
	}

	Entity* createProjectileEntity(Event_createProjectile* e)
	{
		Entity* entity = createEntity();

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
		physics->velocity.x = 1.0f;
		
		return entity;
	}
};


//
// Undefine evil macros
//

#undef CREATE_ATTRIBUTE
#undef CONNECT_ATTRIBUTES