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
	forceActivationState(DISABLE_DEACTIVATION); //Prevent the player from getting stuck when standing still
	return true;
}

void PlayerPhysicsObject::onUpdate(float delta)
{
	handleInput();
}

void PlayerPhysicsObject::handleInput()
{
	std::vector<int> inputAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_INPUT);
	
	Attribute_Input* inputAttribute;

	if(inputAttributes.size() > 1)
	{
		std::cout << "More than one controller for one player. Not tested." << std::endl;
	}

	for(int i=0;i<inputAttributes.size();i++)
	{
		inputAttribute = itrInput.at(inputAttributes.at(i));
	}

	yaw_ += inputAttribute->rotation.x;
	btVector3 move = 5*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	move = move.rotate(btVector3(0,1,0),yaw_);
	move = btVector3(move.x(), getLinearVelocity().y(), move.z());
	setLinearVelocity(move);

	btTransform world;
	world = getWorldTransform();
	world.setRotation(btQuaternion(yaw_,0,0));
	setWorldTransform(world);
}