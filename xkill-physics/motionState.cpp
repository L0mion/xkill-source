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
	A_Ptr<Attribute_Spatial> ptr_spatial = itrPhysics_MotionState.at(attributeIndex_)->ptr_spatial;
	A_Ptr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;
 	worldTrans.setOrigin(convert(ptr_position->position));
	worldTrans.setRotation(convert(ptr_spatial->rotation));
	//DEBUGPRINT("attributeIndex_: " << attributeIndex_ << " MotionState::getWorldTransform");
}

void MotionState::setWorldTransform(const btTransform &worldTrans)
{
	A_Ptr<Attribute_Spatial> ptr_spatial = itrPhysics_MotionState.at(attributeIndex_)->ptr_spatial;
 	A_Ptr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;
	btVector3 position = worldTrans.getOrigin();
	btQuaternion rotation = worldTrans.getRotation();
	ptr_position->position = Float3(position.x(),position.y(),position.z());
	ptr_spatial->rotation = Float4(rotation.x(),rotation.y(),rotation.z(),rotation.w());
	//DEBUGPRINT("attributeIndex_: " << attributeIndex_ << " MotionState::setWorldTransform");
}