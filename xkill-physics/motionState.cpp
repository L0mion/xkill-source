#include "motionState.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "physicsUtilities.h"

AttributeIterator<Attribute_Position> itrPosition_MotionState;
AttributeIterator<Attribute_Spatial> itrSpatial_MotionState;
AttributeIterator<Attribute_Physics> itrPhysics_MotionState;

MotionState::MotionState(unsigned int attributeIndex)
{
	attributeIndex_ = attributeIndex;
	itrSpatial_MotionState = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_MotionState = ATTRIBUTE_MANAGER->position.getIterator();
	itrPhysics_MotionState = ATTRIBUTE_MANAGER->physics.getIterator();
}

MotionState::~MotionState()
{
}

void MotionState::getWorldTransform(btTransform &worldTrans) const
{
	Attribute_Spatial* spatialAttribute = itrSpatial_MotionState.at(itrPhysics_MotionState.at(attributeIndex_)->ptr_spatial);
	Attribute_Position* positionAttribute = itrPosition_MotionState.at(spatialAttribute->ptr_position);
 	worldTrans.setOrigin(convert(positionAttribute->position));
	worldTrans.setRotation(convert(spatialAttribute->rotation));
}

void MotionState::setWorldTransform(const btTransform &worldTrans)
{
	Attribute_Spatial* spatialAttribute = itrSpatial_MotionState.at(itrPhysics_MotionState.at(attributeIndex_)->ptr_spatial);
 	Attribute_Position* positionAttribute = itrPosition_MotionState.at(spatialAttribute->ptr_position);
	btVector3 position = worldTrans.getOrigin();
	btQuaternion rotation = worldTrans.getRotation();
	positionAttribute->position = Float3(position.x(),position.y(),position.z());
	spatialAttribute->rotation = Float4(rotation.x(),rotation.y(),rotation.z(),rotation.w());
}