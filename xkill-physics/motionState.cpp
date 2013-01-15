#include "motionState.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "physicsUtilities.h"

ATTRIBUTES_DECLARE_ALL;
static bool isFirst = true;


MotionState::MotionState()
{
	if(isFirst)
	{
		ATTRIBUTES_INIT_ALL;
		isFirst = false;
	}

	attributeIndex_ = -1;
}

MotionState::~MotionState()
{
}

void MotionState::setAttributeIndex(unsigned int attributeIndex)
{
	attributeIndex_ = attributeIndex;
}

unsigned int MotionState::getAttributeIndex()
{
	return attributeIndex_;
}

void MotionState::getWorldTransform(btTransform &worldTrans) const
{
	Attribute_Spatial* spatialAttribute = itrSpatial.at(itrPhysics.at(attributeIndex_)->ptr_spatial);
	Attribute_Position* positionAttribute = itrPosition.at(spatialAttribute->ptr_position);
 	worldTrans.setOrigin(convert(positionAttribute->position));
	worldTrans.setRotation(convert(spatialAttribute->rotation));
}

void MotionState::setWorldTransform(const btTransform &worldTrans)
{
	Attribute_Spatial* spatialAttribute = itrSpatial.at(itrPhysics.at(attributeIndex_)->ptr_spatial);
 	Attribute_Position* positionAttribute = itrPosition.at(spatialAttribute->ptr_position);
	btVector3 position = worldTrans.getOrigin();
	btQuaternion rotation = worldTrans.getRotation();
	positionAttribute->position = Float3(position.x(),position.y(),position.z());
	spatialAttribute->rotation = Float4(rotation.x(),rotation.y(),rotation.z(),rotation.w());
}