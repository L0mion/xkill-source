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
	Entity* createEntity_TypeA()
	{
		Entity* e = createEntity();

		// Render attribute
		attributeFactory.create_RenderAttribute(e);

		// Return entity
		return e;
	}

	Entity* createEntity_Camera()
	{
		Entity* e = createEntity();

		// Spatial attribute
		attributeFactory.create_RenderAttribute(e);

		// Render attribute
		CameraAttribute* camera = AttributeManager::getInstance()->cameraAttributes_.createAttribute(e);
		AttributeManager::getInstance()->cameraAttributes_.getAllAttributes()->size();
		ZeroMemory(camera->mat_projection, sizeof(camera->mat_projection));
		ZeroMemory(camera->mat_view, sizeof(camera->mat_view));
		camera->spatialAttribute = AttributeManager::getInstance()->spatialAttributes_.createAttributePointer();

		// Return entity
		return e;
	}
	Entity* createPlayerEntity()
	{
		Entity* entity = createEntity();
		
		// Render attribute
		attributeFactory.create_RenderAttribute(entity);

		// Player attribute
		PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes_.createAttribute(entity);
		playerAttribute->name = "LoccaShock";
		playerAttribute->priority = 0;
		playerAttribute->cycleSteals = 0;
		playerAttribute->totalExecutionTime = 0;
		playerAttribute->renderAttribute = AttributeManager::getInstance()->renderAttributes_.createAttributePointer();

		// Return entity
		return entity;
	}
};