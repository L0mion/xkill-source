#include "PlayerPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "physicsUtilities.h"
#include "debugDrawDispatcher.h"
#include "collisionShapes.h"

AttributeIterator<Attribute_Input> itrInput;
AttributeIterator<Attribute_Physics> itrPhysics_3;
AttributeIterator<Attribute_Spatial> itrSpatial;
AttributeIterator<Attribute_Player> itrPlayer;
AttributeIterator<Attribute_Health> itrHealth;
//AttributeIterator<Attribute_Ray> itrRay_PlayerPhysicsObject;

PlayerPhysicsObject::PlayerPhysicsObject()
	: PhysicsObject()
{
	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
	itrPhysics_3 = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
	itrHealth = ATTRIBUTE_MANAGER->health.getIterator();
	//itrRay_PlayerPhysicsObject = ATTRIBUTE_MANAGER->ray.getIterator();

	yaw_ = 0.0f;
	hoverHeight = 1.5f;
	playerCollisionShapeWhenDead = new btCapsuleShape(0.75f, 0.25f);
}

PlayerPhysicsObject::~PlayerPhysicsObject()
{
	delete playerCollisionShapeWhenDead;
}

bool PlayerPhysicsObject::subClassSpecificInitHook()
{
	forceActivationState(DISABLE_DEACTIVATION); //Prevent the player from getting stuck when standing still
	//setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	
	Float4 q = itrPhysics_3.at(attributeIndex_)->ptr_spatial->rotation;
	yaw_ = -atan2(2*q.y*q.w-2*q.x*q.z , 1 - 2*q.y*q.y - 2*q.z*q.z);
	return true;
}

void PlayerPhysicsObject::onUpdate(float delta)
{
	PhysicsObject::onUpdate(delta);

	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerAttributes.at(i));
		               
		if(!ptr_player->detectedAsDead)
		{
			handleInput(delta);
		}

		if( !(ptr_player->jetpack) && !(ptr_player->detectedAsDead) && !(ptr_player->ptr_input->jump))
		{
			hover(delta, hoverHeight);
		}
	}
}

void PlayerPhysicsObject::hover(float delta, float hoverHeight)
{
	float deltaHeightMaximum = 0.0f;
	btVector3 offset[] = {btVector3( 0.15f, 0.0f,  0.15f),
						  btVector3( 0.15f, 0.0f, -0.15f),
						  btVector3(-0.15f, 0.0f,  0.15f),
						  btVector3(-0.15f, 0.0f, -0.15f)};
	for(unsigned int i=0; i<4; i++)
	{
		btVector3 from = btVector3(0.0f, 0.0f, 0.0f);
		btVector3 to = (from - btVector3(0.0f,hoverHeight*2.0f,0.0f)) + offset[i];
		from += offset[i];
		
		//use player spatatial postion instead (2013-03-20 11.01) //check

		from += getWorldTransform().getOrigin();
		to   += getWorldTransform().getOrigin();

		btQuaternion btqt = getWorldTransform().getRotation();
		
		btCollisionWorld::ClosestRayResultCallback ray(from,to);
		ray.m_collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::RAY;
		ray.m_collisionFilterMask = XKILL_Enums::PhysicsAttributeType::WORLD;
		dynamicsWorld_->rayTest(from,to,ray); //cast ray from player position straight down
		if(ray.hasHit())
		{
			btVector3 point = from.lerp(to,ray.m_closestHitFraction);
			float length = (point - from).length();
			float deltaHeight = hoverHeight-length;
			if(deltaHeight > deltaHeightMaximum)
			{
				deltaHeightMaximum = deltaHeight;
			}
		}
		debugDrawer_->drawLine(from, to, btVector3(0.2f, 1.0f, 0.2f));
	}

	bool isHovering = false;

	if(deltaHeightMaximum > 0.0f)
	{
		btTransform worldTransform;
		worldTransform = getWorldTransform();
		worldTransform.setOrigin(worldTransform.getOrigin() + btVector3(0.0f,deltaHeightMaximum,0.0f)*delta/0.25f);
		setWorldTransform(worldTransform);

		setLinearVelocity(getLinearVelocity()+btVector3(0.0f,-getLinearVelocity().y(),0.0f));

		isHovering = true;
	}

	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i=0;i<playerAttributes.size();i++)
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerAttributes.at(i));
		ptr_player->hovering = isHovering;
	}
}

btVector3 PlayerPhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	Entity* playerEntity = itrPhysics_3.ownerAt(attributeIndex_);
	std::vector<int> playerId = playerEntity->getAttributes(ATTRIBUTE_PLAYER);
	bool detectedAsDead = false;

	btVector3 localInertia;
	for(unsigned int i = 0; i < playerId.size(); i++)
	{
		detectedAsDead = itrPlayer.at(playerId.at(i))->detectedAsDead;

		if(detectedAsDead)
		{
			//setCollisionShape(CollisionShapes::Instance()->playerCollisionShapeWhenDead);
			//localInertia = localInertiaBasedOnCollisionShapeAndMass(itrPhysics_3.at(attributeIndex_)->mass);
			//localInertia = localInertiaBasedOnFirstChildShapeOfCompoundCollisionShapeAndMass(itrPhysics_3.at(attributeIndex_)->mass);

			btVector3 localInertia;
			playerCollisionShapeWhenDead->calculateLocalInertia(mass, localInertia);
		}
		else
		{
			localInertia = zeroLocalInertia();
		}
	}

	return localInertia;
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
			if(!SETTINGS->isNullprocessExecuting && ptr_player->priority > 0)
			{
				ptr_player->priority--; //punish players for falling outside of the level, if the null process is not running and if the player has priority
				{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("You were","out of bounds","", "-1 priority"); SEND_EVENT(&e);}
			}
			else
			{
				{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("You were","out of bounds","", ""); SEND_EVENT(&e);}
			}
			DEBUGPRINT("Player entity " << playerEntityIndex << " was out of bounds");
			SEND_EVENT(&Event_PlayerDeath(playerAttributeIndices[i]));
		}
	}
}
#include <xkill-utilities/Converter.h>
void PlayerPhysicsObject::handleInput(float delta)
{
	std::vector<int> playerAttributes = itrPhysics_3.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_PLAYER);
	if(playerAttributes.size() > 1)
	{
		ERROR_MESSAGEBOX("More than one controller for one player. Not tested.")
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

		//Jetpack
		if(ptr_player->jetpack && ptr_player->timeSinceLastDamageTaken > 2.0f)
		{
			float jetpackPower = -getGravity().y()*1.5f;
   			world = getWorldTransform();
			btVector3 velocity = getLinearVelocity();
			if(world.getOrigin().y() < 18.0f)
			{
				setLinearVelocity(btVector3(move.x(), velocity.y()+jetpackPower*delta, move.z()));
			}
		}
		else if(ptr_input->jump && ptr_player->hovering) //Jump
		{
			float jumpPower = 600.0f;
			applyCentralImpulse(btVector3(0.0f, jumpPower, 0.0f));
			//applyCentralForce(btVector3(0.0f, jumpPower, 0.0f));
		}
	}
}

btCollisionShape* PlayerPhysicsObject::subClassSpecificCollisionShape()
{
	return CollisionShapes::Instance()->playerCompoundCollisionShape;
}