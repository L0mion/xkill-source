#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"
#include <vector>

class EntityFactory 
{
public:
	// Create entity and assigns unique ID
	Entity* createEntity()
	{
		static int id = 1;

		Entity* e = new Entity(id);
		id++;

		return e;
	}

	Entity* createEntity_TypeA()
	{
		Entity* e = createEntity();

		// Position attribute
		PositionAttribute* position = AttributeManager::getInstance()->positionAttributes.createAttribute(e);
		position->position		= 0.0f;

		// Spatial attribute
		SpatialAttribute* spatial = AttributeManager::getInstance()->spatialAttributes.createAttribute(e);
		spatial->rotation		= 360;
		spatial->scale			= 1.0f;
		spatial->positionAttribute = AttributeManager::getInstance()->positionAttributes.getAttributePointer();

		// Render attribute
		RenderAttribute* render = AttributeManager::getInstance()->renderAttributes.createAttribute(e);
		render->transparent		= false;
		render->tessellation	= true;
		render->meshID			= e->getID();
		render->textureID		= 42;
		render->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.getAttributePointer();


		// Return entity
		return e;
	}
};