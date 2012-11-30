#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "AttributeManager.h"
#include "AttributeFactory.h"

#include <vector>
#include <iostream>

/// A factory for creating Attributes to speed up Entity creation.
/** 
\ingroup ARCHITECTURE
*/

class AttributeFactory
{
private:
	void fillFloat3(float* f)
	{
		f[0] = 1.0f;
		f[1] = 1.0f;
		f[2] = 1.0f;
	}

	void fillQuaternion(float* f)
	{
		f[0] = 0.0f;
		f[1] = 0.0f;
		f[2] = 0.0f;
		f[3] = 1.0f;
	}

public:
	void create_PositionAttribute(Entity* e)
	{
		// Position attribute
		PositionAttribute* position = AttributeManager::getInstance()->positionAttributes.createAttribute(e);
		fillFloat3(position->position);
	}

	void create_SpatialAttribute(Entity* e)
	{
		// Position attribute
		create_PositionAttribute(e);

		// Spatial attribute
		SpatialAttribute* spatial = AttributeManager::getInstance()->spatialAttributes.createAttribute(e);
		fillQuaternion(spatial->rotation);
		fillFloat3(spatial->scale);
		spatial->positionAttribute = AttributeManager::getInstance()->positionAttributes.getLatestAttributeAsAttributePointer();
	}

	void create_RenderAttribute(Entity* e)
	{
		// Spatial attribute
		create_SpatialAttribute(e);

		// Render attribute
		RenderAttribute* render = AttributeManager::getInstance()->renderAttributes.createAttribute(e);
		render->transparent		= false;
		render->tessellation	= true;
		render->meshID			= e->getID();
		render->textureID		= 42;
		render->spatialAttribute = AttributeManager::getInstance()->spatialAttributes.getLatestAttributeAsAttributePointer();
	}
};