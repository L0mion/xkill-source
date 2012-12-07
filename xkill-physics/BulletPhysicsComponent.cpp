#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>

#include "CollisionShapeManager.h"
#include "physicsObject.h"

#include <xkill-utilities/EventManager.h>

#define SAFE_DELETE(obj)	if(obj != nullptr) { delete obj;		obj = nullptr; }
BulletPhysicsComponent::BulletPhysicsComponent()
{
	inputAttributes_ = nullptr;
	physicsAttributes_ = nullptr;
	boundingAttributes_ = nullptr;
	
	broadphase_ = nullptr;
	collisionConfiguration_ = nullptr;
	dispatcher_ = nullptr;
	solver_ = nullptr;
	dynamicsWorld_ = nullptr;
	physicsObjects_ = nullptr;
	collisionShapeManager_ = nullptr;
	floor_ = nullptr;
}

BulletPhysicsComponent::~BulletPhysicsComponent()
{
	if(physicsObjects_ != nullptr)
	{
		while(physicsObjects_->size() > 0)
		{
			physicsObjects_->at(physicsObjects_->size()-1)->removeFromWorld(dynamicsWorld_);
			SAFE_DELETE(physicsObjects_->at(physicsObjects_->size()-1));
			physicsObjects_->pop_back();
		}
	}
	SAFE_DELETE(physicsObjects_)

	dynamicsWorld_->removeRigidBody(floor_);
	delete floor_->getMotionState();
	delete floor_->getCollisionShape();

	SAFE_DELETE(floor_);

    SAFE_DELETE(solver_);
    SAFE_DELETE(collisionConfiguration_);
    SAFE_DELETE(dispatcher_);
    SAFE_DELETE(broadphase_);
	SAFE_DELETE(dynamicsWorld_);
	SAFE_DELETE(collisionShapeManager_);
}




bool BulletPhysicsComponent::init()
{
	// Fetch attributes
	GET_ATTRIBUTES(inputAttributes_, InputAttribute, ATTRIBUTE_INPUT);
	GET_ATTRIBUTES(physicsAttributes_, PhysicsAttribute, ATTRIBUTE_PHYSICS);
	GET_ATTRIBUTES(boundingAttributes_, BoundingAttribute, ATTRIBUTE_BOUNDING);
	GET_ATTRIBUTE_OWNERS(physicsOwners_, ATTRIBUTE_PHYSICS);

	physicsObjects_ = new btAlignedObjectArray<PhysicsObject*>();
	broadphase_ = new btDbvtBroadphase();
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
	solver_ = new btSequentialImpulseConstraintSolver;
	dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
	collisionShapeManager_ = new CollisionShapeManager();

	dynamicsWorld_->setGravity(btVector3(0,0,0));

	floor_ = new btRigidBody(0,
							 new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0))),
							 new btStaticPlaneShape(btVector3(0,1,0),0),
							 btVector3(0,0,0));
	dynamicsWorld_->addRigidBody(floor_);

	collisionShapeManager_->createConvexHull(nullptr,0);

	return true;
}




void BulletPhysicsComponent::onUpdate(float delta)
{
	for(unsigned int i = 0; i < inputAttributes_->size(); i++)
	{
		if(i < static_cast<unsigned int>(physicsObjects_->size()))
			physicsObjects_->at(inputAttributes_->at(i).physicsAttribute.index)->input(&inputAttributes_->at(i),delta);
	}

	//Checks if new physiscs attributes were created since last call to this function
	for(unsigned int i = physicsObjects_->size(); i < physicsAttributes_->size(); i++)
	{
		physicsObjects_->push_back(new PhysicsObject(collisionShapeManager_));
	}
	
	//Synchronize the internal represenation of physics objects with the physics attributes
	for(unsigned int i = 0; i < static_cast<unsigned int>(physicsObjects_->size()); i++)
	{
		PhysicsObject* physicsObject = physicsObjects_->at(i);
		PhysicsAttribute* physicsAttribute = &physicsAttributes_->at(i);
		if(physicsOwners_->at(i)!=0)
		{
			if(!physicsObject->isInWorld())
			{
				physicsObject->addToWorld(dynamicsWorld_);
			}
			physicsObject->preStep(collisionShapeManager_,physicsAttribute);
		}
		else if(physicsObject->isInWorld())
		{
			physicsObject->removeFromWorld(dynamicsWorld_);
		}
	}
	
	//When data have been tranferred from PhysicsAttributes to the internal representation
	dynamicsWorld_->stepSimulation(delta,10);//Perform Bullet Physics simulation

	//Copy the physics simulation result to the physics attributes
	for(unsigned int i = 0; i < static_cast<unsigned int>(physicsObjects_->size()); i++)
	{
		PhysicsObject* physicsObject = physicsObjects_->at(i);
		PhysicsAttribute* physicsAttribute = &physicsAttributes_->at(i);
		if(physicsOwners_->at(i)!=0 && physicsObject->isInWorld())
		{
			physicsObject->postStep(physicsAttribute);

			//Check collisions between players and projectiles
			for(unsigned int j = i+1; j < physicsObjects_->size(); j++)
			{
				if(physicsAttributes_->at(j).alive && physicsAttributes_->at(j).added)
				{
					//^ = xor. If one of the 2 physics objects (at i and j in physicsAttributes_) is a projectile, and the other object is not a projectile.
					if(physicsAttributes_->at(i).isProjectile ^ physicsAttributes_->at(j).isProjectile)
					{
						//Collision test
						if((*physicsObjects_)[i]->contactTest(dynamicsWorld_,*(*physicsObjects_)[j]))
						{
							std::vector<int>* allPhysicsOwner; GET_ATTRIBUTE_OWNERS(allPhysicsOwner, ATTRIBUTE_PHYSICS);
							int physicsAttributeOwnersI = allPhysicsOwner->at(i);
							int physicsAttributeOwnersJ = allPhysicsOwner->at(j);

							//Find out which one of the 2 physics objects (at i and j in physicsAttributes_) that is a projectile.
							int projectileEntityId = -1;
							int playerEntityId = -1;
							if(physicsAttributes_->at(i).isProjectile)
							{
								physicsAttributes_->at(i).alive = false;
								projectileEntityId = physicsAttributeOwnersI;
								playerEntityId = physicsAttributeOwnersJ;
							}
							else if(physicsAttributes_->at(j).isProjectile)
							{
								physicsAttributes_->at(j).alive = false;
								playerEntityId = physicsAttributeOwnersI;
								projectileEntityId = physicsAttributeOwnersJ;
							}

							Event_ProjectileCollidingWithPlayer projectileCollidingWithPlayer(projectileEntityId, playerEntityId);
							SEND_EVENT(&projectileCollidingWithPlayer);
						}
					}
				}
			}
		}
	}
}

void BulletPhysicsComponent::onEvent(Event* e)
{

}
