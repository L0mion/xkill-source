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
 	worldTrans.setOrigin(convert(spatialAttribute->position()));
	worldTrans.setRotation(convert(spatialAttribute->rotation()));
	//DEBUGPRINT("attributeIndex_: " << attributeIndex_ << " MotionState::getWorldTransform");
}

void MotionState::setWorldTransform(const btTransform &worldTrans)
{
	Attribute_Spatial* spatialAttribute = itrSpatial_MotionState.at(itrPhysics_MotionState.at(attributeIndex_)->ptr_spatial);
	btVector3 position = worldTrans.getOrigin();
	btQuaternion rotation = worldTrans.getRotation();
	spatialAttribute->setPosition( Float3(position.x(),position.y(),position.z()));
	spatialAttribute->setRotation( Float4(rotation.x(),rotation.y(),rotation.z(),rotation.w()));
	//DEBUGPRINT("attributeIndex_: " << attributeIndex_ << " MotionState::setWorldTransform");
}