#pragma once

#include "AttributeType.h"

class AttributeManager
{
public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute> spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}
};