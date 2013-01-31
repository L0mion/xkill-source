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
	
	
	A_Ptr<Attribute_Input> ptr_input;
	Attribute_Player* ptr_player;

	if(playerAttributes.size() > 1)
	{
		//std::cout << "More than one controller for one player. Not tested." << std::endl;
	}

	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		ptr_player = itrPlayer.at(playerAttributes.at(i));

		A_Ptr<Attribute_Health> health = ptr_player->ptr_health;
		if(health->health <= 0)
			continue;

		ptr_input = ptr_player->ptr_input;

		//look and move
		yaw_ += ptr_input->rotation.x;
		btVector3 move = ptr_player->currentSpeed*btVector3(ptr_input->position.x, 0, ptr_input->position.y);
		move = move.rotate(btVector3(0,1,0),yaw_);
		move = btVector3(move.x(), getLinearVelocity().y(), move.z());
		setLinearVelocity(move);

		btTransform world;
		world = getWorldTransform();
		world.setRotation(btQuaternion(yaw_,0,0));
		setWorldTransform(world);

		//Jump
		if(ptr_input->jump && ptr_player->timeSinceLastJump > ptr_player->delayInSecondsBetweenEachJump && ptr_player->collidingWithWorld)
		{
			applyCentralImpulse(btVector3(0.0f, 5.0f, 0.0f));
			ptr_player->timeSinceLastJump = 0.0f;
		}

		//Jetpack
		if(ptr_input->jetpack)
		{
			applyCentralImpulse(btVector3(0.0f, 50.0f*delta, 0.0f));
			ptr_player->jetpackTimer+=delta;
			if(ptr_player->jetpackTimer > 0.1f)
			{
				ptr_player->jetpackTimer = 0.0f;
				health->health--;
			}
		}

		Attribute_Physics* playerPhysicsAttribute = itrPhysics_3.at(attributeIndex_);

		//Prevent player from sliding down slopes
		if(ptr_input->position.x == 0.0f && ptr_input->position.y == 0.0f && ptr_player->collidingWithWorld && !ptr_input->jetpack && !ptr_input->jump)
		{
			setFriction(btScalar(100.0f)); //Friction when player are standing still
			setGravity(btVector3(0.0f, 0.0f, 0.0f));
		}
		else //restore friction and gravity
		{
			setFriction(btScalar(0.0f));
			setGravity(btVector3(playerPhysicsAttribute->gravity.x, playerPhysicsAttribute->gravity.y, playerPhysicsAttribute->gravity.z));
		}

		ptr_input->jump = false;
		ptr_input->jetpack = false;
	}
}