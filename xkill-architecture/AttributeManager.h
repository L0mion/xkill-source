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
	AttributeStorage<PositionAttribute> positionAttributes_;
	AttributeStorage<SpatialAttribute>  spatialAttributes_;
	AttributeStorage<RenderAttribute>	renderAttributes_;
	AttributeStorage<PhysicsAttribute>	physicsAttributes_;
	AttributeStorage<CameraAttribute>	cameraAttributes_;
	AttributeStorage<InputAttribute>	inputAttributes_;
	AttributeStorage<PlayerAttribute>	playerAttributes_;
	AttributeStorage<BoundingAttribute> boundingAttributes_;


	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}
};