#pragma once

#include <xkill-utilities/AttributeType.h>
#include "AttributeStorage.h"

/// Singleton responsible for providing access to each Attribute.
/** 
\ingroup ARCHITECTURE
*/

class AttributeManager
{
public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute>	spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;
	AttributeStorage<PhysicsAttribute>	physicsAttributes;
	AttributeStorage<CameraAttribute>	cameraAttributes;
	AttributeStorage<InputAttribute>	inputAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}
};