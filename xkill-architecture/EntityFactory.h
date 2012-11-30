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

		// Camera attribute
		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes.createAttribute(e);
		AttributeManager::getInstance()->cameraAttributes.getAllAttributes()->size();
		ZeroMemory(camera->mat_projection, sizeof(camera->mat_projection));
		ZeroMemory(camera->mat_view, sizeof(camera->mat_view));
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.getLatestAttributeAsAttributePointer();

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

		// Camera attribute
		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes.createAttribute(entity);
		ZeroMemory(camera->mat_projection, sizeof(camera->mat_projection));
		ZeroMemory(camera->mat_view, sizeof(camera->mat_view));
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.getLatestAttributeAsAttributePointer(); //Bind the last created spatial attribute to the camera attribute (NOTE: the last created spatial attribute should be the same attribute as bound to the player attribute)

		// Player attribute
		PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes.createAttribute(entity);
		playerAttribute->id = 0;
		playerAttribute->name = "PrinterTerror";
		playerAttribute->health = 100;
		playerAttribute->priority = 0;
		playerAttribute->cycleSteals = 0;
		playerAttribute->totalExecutionTime = 0;
		playerAttribute->renderAttribute = AttributeManager::getInstance()->renderAttributes.getLatestAttributeAsAttributePointer(); //Bind the last created render attribute to the player attribut

		//Increment local static variable
		playerId++;

		// Return entity
		return entity;
	}
};