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

void PlayerPhysicsObject::onUpdate(float delta,btDynamicsWorld* dynamicWorld)
{
	PhysicsObject::onUpdate(delta,dynamicWorld);

	handleInput(delta);

	btVector3 from = getWorldTransform().getOrigin();
	btVector3 to = from - btVector3(0,50,0);
	btCollisionWorld::ClosestRayResultCallback ray(from,to);
	ray.m_collisionFilterGroup = Attribute_Physics::PhysicsAttributeType::RAY;
	ray.m_collisionFilterMask =  Attribute_Physics::PhysicsAttributeType::WORLD;
	dynamicWorld->rayTest(from,to,ray);
	if(ray.hasHit())
	{
		btVector3 point = from.lerp(to,ray.m_closestHitFraction);
		float length = (point - from).length();
		float height = 1;
		if(height-length > 0)
		{
			btTransform t;
			t= getWorldTransform();
			t.setOrigin(t.getOrigin() + btVector3(0,height-length,0)*delta/0.25);
			setWorldTransform(t);
			setLinearVelocity(getLinearVelocity()+btVector3(0,-getLinearVelocity().y(),0));
			//applyCentralForce(-getGravity());
		}
	}
	//Handle players taking off when going up ramps
	/*Entity* playerEntity = itrPhysics_3.ownerAt(attributeIndex_);
	std::vector<int> playerAttributeIndices = playerEntity->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i = 0; i < playerAttributeIndices.size(); i++)
	{
		AttributePtr<Attribute_Player> playerAttribute = itrPlayer.at(playerAttributeIndices.at(i));
		if(!playerAttribute->collidingWithWorld && playerAttribute->timeSinceLastJump > playerAttribute->delayInSecondsBetweenEachJump && getLinearVelocity().y() > 0.0f)
		{
			setLinearVelocity(btVector3(getLinearVelocity().x(), 0.0f, getLinearVelocity().z()));
		}
		playerAttribute->collidingWithWorld = false;
	}*/
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
			DEBUGPRINT("Player entity " << playerEntityIndex << " was out of bounds");
			SEND_EVENT(&Event_PlayerDeath(playerAttributeIndices[i]));
		}
	}
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
	
	
	AttributePtr<Attribute_Input> ptr_input;
	AttributePtr<Attribute_Player> ptr_player;

	if(playerAttributes.size() > 1)
	{
		SHOW_MESSAGEBOX("More than one controller for one player. Not tested.")
	}

	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		ptr_player = itrPlayer.at(playerAttributes.at(i));

		AttributePtr<Attribute_Health> health = ptr_player->ptr_health;
		if(health->health <= 0)
			continue;

		ptr_input = ptr_player->ptr_input;

		//look and move
		yaw_ += ptr_input->rotation.x;
		btVector3 move = ptr_player->currentSpeed*btVector3(ptr_input->position.x, 0, ptr_input->position.y);

		//lower player speed when recently damaged
		if(ptr_player->timeSinceLastDamageTaken < 1.0f)
		{
			move *= 0.75f;
		}

		//Airwalk handling
		if(!ptr_player->collidingWithWorld)
		{
			move *= 0.75f;
		}
		move = move.rotate(btVector3(0,1,0),yaw_);
		move = btVector3(move.x(), getLinearVelocity().y(), move.z());
		setLinearVelocity(move);

		btTransform world;
		world = getWorldTransform();
		world.setRotation(btQuaternion(yaw_,0,0));
		setWorldTransform(world);

		//Jump
		float jumpPower = 10.0f;
		if(ptr_input->jump && ptr_player->timeSinceLastJump > ptr_player->delayInSecondsBetweenEachJump && ptr_player->collidingWithWorld)
		{
			applyCentralImpulse(btVector3(0.0f, jumpPower, 0.0f));
			ptr_player->timeSinceLastJump = 0.0f;
		}

		//Jetpack
		if(ptr_input->jetpack)
		{
			applyCentralImpulse(btVector3(0.0f, jumpPower*10.0f*delta, 0.0f));
			ptr_player->jetpackTimer+=delta;
			if(ptr_player->jetpackTimer > 0.1f)
			{
				ptr_player->jetpackTimer = 0.0f;
				health->health--;
			}
		}

		AttributePtr<Attribute_Physics> ptr_player_physics = itrPhysics_3.at(attributeIndex_);
		btVector3 currentplayerGravity = getGravity();

		////When a player is standing still on the ground, prevent it from sliding down slopes by modifying friction and gravity
		//if(ptr_input->position.x == 0.0f && ptr_input->position.y == 0.0f && ptr_player->collidingWithWorld && !ptr_input->jetpack && !ptr_input->jump)
		//{
		//	if(currentplayerGravity.y() != 0.0f)
		//	{
		//		setFriction(btScalar(100.0f));
		//		setGravity(btVector3(0.0f, 0.0f, 0.0f));
		//	}
		//}
		////When moving, restore friction and gravity
		//else if( (ptr_input->position.x != 0.0f || ptr_input->position.y != 0.0f))
		//{
		//	if(currentplayerGravity.y() != ptr_player_physics->gravity.y)
		//	{
		//		setFriction(btScalar(0.0f));
		//		setGravity(btVector3(ptr_player_physics->gravity.x, ptr_player_physics->gravity.y, ptr_player_physics->gravity.z));
		//	}
		//}

		////Prevent player being able to hang-glide after jumping
		//if(ptr_player->timeSinceLastJump < ptr_player->delayInSecondsBetweenEachJump)
		//{
		//	setGravity(btVector3(0.0f, ptr_player_physics->gravity.y*5.0f, 0.0f));
		//}

		//ptr_input->jump = false;
		//ptr_input->jetpack = false;
	}
}