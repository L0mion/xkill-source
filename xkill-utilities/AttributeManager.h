#pragma once

#include "AttributeStorage.h"
#include "AttributeType.h"

class AttributeManager
{
public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute> spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;

	static AttributeManager* getInstance();
};

