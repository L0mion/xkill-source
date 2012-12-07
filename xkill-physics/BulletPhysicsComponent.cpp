#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>

#include "CollisionShapeManager.h"
#include "physicsObject.h"

#include <xkill-utilities/EventManager.h>

#include <iostream>

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
	delete floor_->getCollisionShape();

	SAFE_DELETE(floor_);

    SAFE_DELETE(solver_);
    SAFE_DELETE(collisionConfiguration_);
    SAFE_DELETE(dispatcher_);
    SAFE_DELETE(broadphase_);
	SAFE_DELETE(dynamicsWorld_);
	SAFE_DELETE(collisionShapeManager_);
}

void wrapTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    BulletPhysicsComponent *component = static_cast<BulletPhysicsComponent *>(world->getWorldUserInfo());
	component->tickCallback(timeStep);
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
	dynamicsWorld_->setInternalTickCallback(wrapTickCallback,static_cast<void*>(this));


	floor_ = new PhysicsObject(new btStaticPlaneShape(btVector3(0,1,0),0),1337);
	dynamicsWorld_->addRigidBody(floor_);

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
		physicsObjects_->push_back(new PhysicsObject(collisionShapeManager_,i));
	}
	
	//Synchronize the internal represenation of physics objects with the physics attributes
	for(unsigned int i = 0; i < static_cast<unsigned int>(physicsObjects_->size()); i++)
	{
		PhysicsObject* physicsObject = physicsObjects_->at(i);
		PhysicsAttribute* physicsAttribute = &physicsAttributes_->at(i);
		// if the objects owner is not 0 it should simulate
		if(physicsOwners_->at(i)!=0)
		{
			// if the object is not in the world, add it
			if(!physicsObject->isInWorld())
			{
				physicsObject->addToWorld(dynamicsWorld_);
			}
			// load data from physics attribute
			physicsObject->preStep(collisionShapeManager_,physicsAttribute);
		}
		else if(physicsObject->isInWorld())
		{
			// if the objects owner is 0 then remove it from simulation world
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
		}
	}
}

void BulletPhysicsComponent::onEvent(Event* e)
{

}

void BulletPhysicsComponent::tickCallback(btScalar timeStep)
{
	btPersistentManifold* persistentManifold;
	btDispatcher* dispatcher = dynamicsWorld_->getDispatcher();
	unsigned int numManifolds = dynamicsWorld_->getDispatcher()->getNumManifolds();
	// loop through all manifolds of from last timestep
	for(unsigned int i = 0; i < numManifolds; i++)
	{
		persistentManifold = dispatcher_->getManifoldByIndexInternal(i);
		unsigned int numContacts = persistentManifold->getNumContacts();
		// loop through all contact points of manifold
		for(unsigned int j = 0; j < numContacts; j++)
		{
			// ignore contacts where distance is larger than 0
			if(persistentManifold->getContactPoint(j).getDistance()<=0.0f)
			{
				const PhysicsObject* objectA = static_cast<const PhysicsObject*>(persistentManifold->getBody0());
				const PhysicsObject* objectB = static_cast<const PhysicsObject*>(persistentManifold->getBody1());
				//std::cout << std::endl << objectA->getIndex() << " " << objectB->getIndex() << " " << physicsOwners_->size();
				if( objectA->getIndex() != 1337 && objectB->getIndex() != 1337)
				{
					unsigned int ownerA = physicsOwners_->at(objectA->getIndex());
					unsigned int ownerB = physicsOwners_->at(objectB->getIndex());
					//std::cout << std::endl << ownerA << " " << ownerB;
					// ignore contacts where one owner is B
					if(ownerA != 0 && ownerB != 0)
					{
						//std::cout << "\nCollision between " << ownerA << " & " << ownerB;
						// if one of the object is a projectile send an event with the projectile owner as first argument and the hitee as second
						if(physicsAttributes_->at(objectA->getIndex()).isProjectile)
						{
							Event_ProjectileCollidingWithPlayer projectileCollidingWithPlayer(ownerA,ownerB);
							SEND_EVENT(&projectileCollidingWithPlayer);
							break;
						}
						else if(physicsAttributes_->at(objectB->getIndex()).isProjectile)
						{
							Event_ProjectileCollidingWithPlayer projectileCollidingWithPlayer(ownerB,ownerA);
							SEND_EVENT(&projectileCollidingWithPlayer);
							break;
						}
					}
				}
			}
		}
	}
}