#include "PlayerPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>

#include "MotionState.h"

AttributeIterator<Attribute_Input> itrInput;
AttributeIterator<Attribute_Physics> itrPhysics_3;
AttributeIterator<Attribute_Spatial> itrSpatial;
AttributeIterator<Attribute_Player> itrPlayer;

PlayerPhysicsObject::PlayerPhysicsObject()
	: PhysicsObject()
{
	yaw_ = 0;

	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
	itrPhysics_3 = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
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
	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	
	Attribute_Input* inputAttribute;
	Attribute_Player* playerAttribute;

	if(playerAttributes.size() > 1)
	{
		//std::cout << "More than one controller for one player. Not tested." << std::endl;
	}

	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		playerAttribute = itrPlayer.at(playerAttributes.at(i));
		inputAttribute = itrInput.at(playerAttribute->ptr_input);
	}

	yaw_ += inputAttribute->rotation.x;
	btVector3 move = playerAttribute->currentSpeed*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	move = move.rotate(btVector3(0,1,0),yaw_);
	move = btVector3(move.x(), getLinearVelocity().y(), move.z());
	setLinearVelocity(move);

	btTransform world;
	world = getWorldTransform();
	world.setRotation(btQuaternion(yaw_,0,0));
	setWorldTransform(world);
}