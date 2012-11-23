#pragma once

#include <xkill-utilities/AttributeType.h>
#include "AttributeStorage.h"

class AttributeManager
{
public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute> spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;
	AttributeStorage<PhysicsAttribute>	physicsAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}
};