#include "PlayerPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>
#include "physicsUtilities.h"

#include "MotionState.h"

AttributeIterator<Attribute_Input> itrInput;
AttributeIterator<Attribute_Physics> itrPhysics_3;
AttributeIterator<Attribute_Spatial> itrSpatial;
AttributeIterator<Attribute_Player> itrPlayer;
AttributeIterator<Attribute_Health> itrHealth;

PlayerPhysicsObject::PlayerPhysicsObject()
	: PhysicsObject()
{
	yaw_ = 0;

	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
	itrPhysics_3 = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
	itrHealth = ATTRIBUTE_MANAGER->health.getIterator();
}

PlayerPhysicsObject::~PlayerPhysicsObject()
{
}

bool PlayerPhysicsObject::subClassSpecificInitHook()
{
	forceActivationState(DISABLE_DEACTIVATION); //Prevent the player from getting stuck when standing still
	//setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	return true;
}

void PlayerPhysicsObject::onUpdate(float delta)
{
	handleInput(delta);
}

void PlayerPhysicsObject::handleInput(float delta)
{
	//btTransform world2;
	//world2 = getWorldTransform();
	//world2.setRotation(btQuaternion(itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.x,
	//							   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.y,
	//							   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.z,
	//							   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.w));
	//setWorldTransform(world2);
	//return;
	/*float yaw = yawFromQuaternion(btQuaternion(itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.x,
											   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.y,
											   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.z,
											   itrSpatial.at(itrPhysics_3.at(attributeIndex_)->ptr_spatial)->rotation.w));*/

	// New code above


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

		Attribute_Health* health = itrHealth.at(playerAttribute->ptr_health);
		if(health->health <= 0)
			continue;

		inputAttribute = itrInput.at(playerAttribute->ptr_input);

		yaw_ += inputAttribute->rotation.x;
		btVector3 move = playerAttribute->currentSpeed*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
		move = move.rotate(btVector3(0,1,0),yaw_);
		move = btVector3(move.x(), getLinearVelocity().y(), move.z());
		setLinearVelocity(move);

		btTransform world;
		world = getWorldTransform();
		world.setRotation(btQuaternion(yaw_,0,0));
		setWorldTransform(world);

		if(inputAttribute->jump && playerAttribute->timeSinceLastJump > playerAttribute->delayInSecondsBetweenEachJump)
		{
			applyCentralImpulse(btVector3(0.0f, 5.0f, 0.0f));
			playerAttribute->timeSinceLastJump = 0.0f;
		}

		if(inputAttribute->jetpack)
		{
			applyCentralImpulse(btVector3(0.0f, 50.0f*delta, 0.0f));
			playerAttribute->jetpackTimer+=delta;
			if(playerAttribute->jetpackTimer > 0.1f)
			{
				health->health--;
			}
		}

		inputAttribute->jump = false;
		inputAttribute->jetpack = false;

	}
}