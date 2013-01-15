#include "PlayerPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>

#include "MotionState.h"

AttributeIterator<Attribute_Input> itrInput;
AttributeIterator<Attribute_Physics> itrPhysics_3;
AttributeIterator<Attribute_Spatial> itrSpatial;

PlayerPhysicsObject::PlayerPhysicsObject()
	: PhysicsObject()
{
	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
	itrPhysics_3 = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial = ATTRIBUTE_MANAGER->spatial.getIterator();
}

PlayerPhysicsObject::~PlayerPhysicsObject()
{
}

bool PlayerPhysicsObject::subClassSpecificInitHook()
{
	forceActivationState(DISABLE_DEACTIVATION);
	return true;
}

void PlayerPhysicsObject::onUpdate(float delta)
{
	handleInput();
}

void PlayerPhysicsObject::handleInput()
{
	std::vector<int> inputAttributes = itrPhysics_3.ownerAt(static_cast<MotionState*>(getMotionState())->getAttributeIndex())->getAttributes(ATTRIBUTE_INPUT);
	Attribute_Input* inputAttribute = itrInput.at(inputAttributes.at(0));

	yaw_ += inputAttribute->rotation.x;
	btVector3 move = btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	move = move.rotate(btVector3(0,1,0),yaw_);
	move = btVector3(move.x(), getLinearVelocity().y(), move.z());
	setLinearVelocity(move);

	btTransform world;
	getMotionState()->getWorldTransform(world);
	world.setRotation(btQuaternion(yaw_,0,0));
	//This does not work:
	//getMotionState()->setWorldTransform(world);

	//MotionState "temporary" (2013-01-15, 16.30 CET) workaround:
	setWorldTransform(world);

	//Consider this code:
	//int index = static_cast<MotionState*>(getMotionState())->getAttributeIndex();
	//btQuaternion q(yaw_,0,0);
	//Attribute_Physics* physicsAttribute = itrPhysics_3.at(index);
	//itrSpatial.at(physicsAttribute->ptr_spatial)->rotation = Float4(q.x(), q.y(), q.z(), q.w());
}