#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"
#include "AttributeFactory.h"

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
	AttributeFactory attributeFactory;	//!< AttributeFactory to speed up creaton of \ref ATTRIBUTES.

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
	//! Example entity
	Entity* createEntity_TypeA()
	{
		Entity* e = createEntity();

		// Render attribute
		attributeFactory.create_RenderAttribute(e);

		// Return entity
		return e;
	}

	//! Not used for now (refer to createPlayerEntity)
	Entity* createEntity_Camera()
	{
		Entity* e = createEntity();

		// Render attribute
		attributeFactory.create_RenderAttribute(e);

		// Render attribute
		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes_.createAttribute(e);
		AttributeManager::getInstance()->cameraAttributes_.getAllAttributes()->size();
		ZeroMemory(camera->mat_projection, sizeof(camera->mat_projection));
		ZeroMemory(camera->mat_view, sizeof(camera->mat_view));
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer();

		// Return entity
		return e;
	}

	//! A player entity has the following attributes: position attribute, spatial attribute, render attribute, player attribute and camera attribute.
	//! Bindings:
	//! camera attribute --> spatial attribute --> position attribute
	//! player attribute --> render attribute --> spatial attribute --> position attribute
	//! Note: the player has the same spatial attribute as the camera.
	Entity* createPlayerEntity()
	{
		static int playerId = 0;
		Entity* entity = createEntity();
		
		// Render attribute
		attributeFactory.create_RenderAttribute(entity);

		// Physics attribute
		attributeFactory.create_PhysicsAttribute(entity);

		//Input attribute
		InputAttribute* input  = AttributeManager::getInstance()->inputAttributes_.createAttribute(entity);
		ZeroMemory(&input->position,(2*sizeof(float)));
		ZeroMemory(&input->rotation,(2*sizeof(float)));
		input->physicsAttribute = AttributeManager::getInstance()->physicsAttributes_.getLatestAttributeAsAttributePointer();

		// Camera attribute
		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes_.createAttribute(entity);
		ZeroMemory(camera->mat_projection, sizeof(camera->mat_projection));
		ZeroMemory(camera->mat_view, sizeof(camera->mat_view));
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.getLatestAttributeAsAttributePointer(); //Bind the last created spatial attribute to the camera attribute (NOTE: the last created spatial attribute should be the same attribute as bound to the player attribute)

		// Player attribute
		PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes_.createAttribute(entity);
		playerAttribute->id = playerId;
		playerAttribute->name = "PrinterTerror";
		playerAttribute->health = 100;
		playerAttribute->priority = 0;
		playerAttribute->cycleSteals = 0;
		playerAttribute->totalExecutionTime = 0;
		playerAttribute->renderAttribute = AttributeManager::getInstance()->renderAttributes_.getLatestAttributeAsAttributePointer(); //Bind the last created render attribute to the player attribut
		playerAttribute->inputAttribute = AttributeManager::getInstance()->inputAttributes_.getLatestAttributeAsAttributePointer();

		//Increment local static variable
		playerId++;

		// Return entity
		return entity;
	}

	Entity* createProjectileEntity()
	{
		Entity* entity = createEntity();

		attributeFactory.create_RenderAttribute(entity);
		attributeFactory.create_PhysicsAttribute(entity);

		return entity;
	}
};