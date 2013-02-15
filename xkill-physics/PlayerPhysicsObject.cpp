#include "PlayerPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "physicsUtilities.h"

AttributeIterator<Attribute_Input> itrInput;
AttributeIterator<Attribute_Physics> itrPhysics_3;
AttributeIterator<Attribute_Spatial> itrSpatial;
AttributeIterator<Attribute_Player> itrPlayer;
AttributeIterator<Attribute_Health> itrHealth;
//AttributeIterator<Attribute_Ray> itrRay_PlayerPhysicsObject;

PlayerPhysicsObject::PlayerPhysicsObject()
	: PhysicsObject()
{
	yaw_ = 0;

	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
	itrPhysics_3 = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
	itrHealth = ATTRIBUTE_MANAGER->health.getIterator();
	//itrRay_PlayerPhysicsObject = ATTRIBUTE_MANAGER->ray.getIterator();
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
	PhysicsObject::onUpdate(delta);

	handleInput(delta);
	float height = 1;
	//--------------------------------------------------------------------------------------
	// Hovering
	//-------------------------------------------------------------------------------------
	
	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerAttributes.at(i));
		if(!(ptr_player->jetpack))
		{
			Hover(delta, 1.0f);
		}
	}
}

void PlayerPhysicsObject::handleOutOfBounds()
{
	setGravity(btVector3(0.0f, 0.0f, 0.0f));
	setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));

	int playerEntityIndex = itrPhysics_3.ownerIdAt(attributeIndex_);
	Entity* playerEntity = itrPhysics_3.ownerAt(attributeIndex_);
				
	std::vector<int> playerAttributeIndices = playerEntity->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i = 0; i < playerAttributeIndices.size(); i++)
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerAttributeIndices.at(i));
		AttributePtr<Attribute_Health> playerHealthAttribute = ptr_player->ptr_health;
		if(!ptr_player->detectedAsDead)
		{
			ptr_player->priority--;
			DEBUGPRINT("Player entity " << playerEntityIndex << " was out of bounds");
			SEND_EVENT(&Event_PlayerDeath(playerAttributeIndices[i]));
		}
	}
}

void PlayerPhysicsObject::handleInput(float delta)
{
	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	if(playerAttributes.size() > 1)
	{
		SHOW_MESSAGEBOX("More than one controller for one player. Not tested.")
	}
	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		AttributePtr<Attribute_Player> ptr_player = ptr_player = itrPlayer.at(playerAttributes.at(i));
		AttributePtr<Attribute_Input> ptr_input = ptr_player->ptr_input;
		AttributePtr<Attribute_Health> health = ptr_player->ptr_health;
		if(health->health <= 0)
		{
			continue;
		}

		//--------------------------------------------------------------------------------------
		//Look and move
		//--------------------------------------------------------------------------------------
		yaw_ += ptr_input->rotation.x;
		btVector3 move = ptr_player->currentSpeed*btVector3(ptr_input->position.x, 0, ptr_input->position.y);

		//lower player speed when recently damaged
		if(ptr_player->timeSinceLastDamageTaken < 1.0f)
		{
			move *= 0.75f;
		}

		//Move player
		move = move.rotate(btVector3(0,1,0),yaw_);
		move = btVector3(move.x(), getLinearVelocity().y(), move.z());
		setLinearVelocity(move);

		//Rotate player
		btTransform world;
		world = getWorldTransform();
		world.setRotation(btQuaternion(yaw_,0,0));
		setWorldTransform(world);

		//Jump, does not work. May not be needed or wanted when the player is already hovering (2013-02-15 11.18)
		//float jumpPower = 10000.0f;
		//if(ptr_input->jump)
		//{
		//	applyCentralImpulse(btVector3(0.0f, jumpPower, 0.0f));
		//}

		//Jetpack
		if(ptr_player->jetpack)
		{
			float jetpackPower = -getGravity().y()*1.5f;
			world = getWorldTransform();
			btVector3 velocity = getLinearVelocity();
			if(world.getOrigin().y() < 8.0f)
			{
				setLinearVelocity(btVector3(move.x(), velocity.y()+jetpackPower*delta, move.z()));
			}
		}
	}
}