#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"

#include <vector>
#include <iostream>

/// A factory for creating Entities and assigning multiple attributes in a flexible way.
/** 
Filling out each Attribute and connecting chains of 
Attribute is the responsibility of the EntityFactory.

\ingroup ARCHITECTURE
*/

class EntityFactory 
{
private:
	/** 
	Creates an entity and assigns a unique ID
	*/
	Entity* EntityFactory::createEntity()
	{
		static int id = 1;

		Entity* e = new Entity(id);
		id++;

		return e;
	}

public:

	Entity* EntityFactory::createEntity_TypeA()
	{
		Entity* e = createEntity();

		// Position attribute
		PositionAttribute* position = AttributeManager::getInstance()->positionAttributes.createAttribute(e);
		position->positionX		= 0.0f;

		// Spatial attribute
		SpatialAttribute* spatial = AttributeManager::getInstance()->spatialAttributes.createAttribute(e);
		spatial->rotation		= 360;
		spatial->scale			= 1.0f;
		spatial->positionAttribute = AttributeManager::getInstance()->positionAttributes.createAttributePointer();

		// Render attribute
		RenderAttribute* render = AttributeManager::getInstance()->renderAttributes.createAttribute(e);
		render->transparent		= false;
		render->tessellation	= true;
		render->meshID			= e->getID();
		render->textureID		= 42;
		render->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.createAttributePointer();


		// Return entity
		std::cout << "ENTITYFACTORY: Created Entity " << e->getID() << std::endl;
		return e;
	}

	Entity* EntityFactory::createPlayerEntity()
	{
		Entity* entity = createEntity();
		
		// Position attribute
		PositionAttribute* position = AttributeManager::getInstance()->positionAttributes.createAttribute(entity);
		position->positionX = 0.0f;
		position->positionY = 0.0f;
		position->positionZ = 0.0f;

		// Spatial attribute
		SpatialAttribute* spatial = AttributeManager::getInstance()->spatialAttributes.createAttribute(entity);
		spatial->rotation = 0;
		spatial->scale = 1.0f;
		spatial->positionAttribute = AttributeManager::getInstance()->positionAttributes.createAttributePointer();

		// Render attribute
		RenderAttribute* render = AttributeManager::getInstance()->renderAttributes.createAttribute(entity);
		render->transparent = false;
		render->tessellation = true;
		render->meshID = entity->getID();
		render->textureID = -1;
		render->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.createAttributePointer();

		// Player attribute
		PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes.createAttribute(entity);
		playerAttribute->name = "LoccaShock";
		playerAttribute->priority = 0;
		playerAttribute->cycleSteals = 0;
		playerAttribute->totalExecutionTime = 0;
		playerAttribute->renderAttribute = AttributeManager::getInstance()->renderAttributes.createAttributePointer();

		return entity;
	}
};