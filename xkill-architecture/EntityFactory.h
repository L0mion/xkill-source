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
		static int playerId = 0;
		Entity* entity = createEntity();

		PositionAttribute* position = AttributeManager::getInstance()->positionAttributes_.createAttribute(entity);
		
		SpatialAttribute* spatial = AttributeManager::getInstance()->spatialAttributes_.createAttribute(entity);
		spatial->positionAttribute = AttributeManager::getInstance()->positionAttributes_.getLatestAttributeAsAttributePointer();
		
		RenderAttribute* render = AttributeManager::getInstance()->renderAttributes_.createAttribute(entity);
		render->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer();

		PhysicsAttribute* physics = AttributeManager::getInstance()->physicsAttributes_.createAttribute(entity);
		physics->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer();
		
		InputAttribute* input  = AttributeManager::getInstance()->inputAttributes_.createAttribute(entity);
		input->physicsAttribute = AttributeManager::getInstance()->physicsAttributes_.getLatestAttributeAsAttributePointer();

		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes_.createAttribute(entity);
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer(); //Bind the most recently created spatial attribute to the camera attribute (NOTE: the last created spatial attribute should be the same attribute as bound to the player attribute)

		PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes_.createAttribute(entity);
		playerAttribute->renderAttribute = AttributeManager::getInstance()->renderAttributes_.getLatestAttributeAsAttributePointer(); //Bind the most recently created render attribute to the player attribute
		playerAttribute->inputAttribute = AttributeManager::getInstance()->inputAttributes_.getLatestAttributeAsAttributePointer();

		//Increment local static variable
		playerId++;

		// Return entity
		return entity;
	}

	Entity* createProjectileEntity(Event_createProjectile* e)
	{
		Entity* entity = createEntity();

		PositionAttribute* positionAttribute = AttributeManager::getInstance()->positionAttributes_.createAttribute(entity);
		positionAttribute->position[0] = e->position3[0];
		positionAttribute->position[1] = e->position3[1];
		positionAttribute->position[2] = e->position3[2];

		SpatialAttribute* spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.createAttribute(entity);
		spatialAttribute->positionAttribute = AttributeManager::getInstance()->positionAttributes_.getLatestAttributeAsAttributePointer();
		spatialAttribute->rotation[0] = e->direction4[0];
		spatialAttribute->rotation[1] = e->direction4[1];
		spatialAttribute->rotation[2] = e->direction4[2];
		spatialAttribute->rotation[3] = e->direction4[3];

		RenderAttribute* renderAttribute = AttributeManager::getInstance()->renderAttributes_.createAttribute(entity);
		renderAttribute->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer();

		PhysicsAttribute* physicsAttribute = AttributeManager::getInstance()->physicsAttributes_.createAttribute(entity);
		physicsAttribute->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer();
		
		physicsAttribute->velocity[0] = 1.0f;
		
		return entity;
	}
};