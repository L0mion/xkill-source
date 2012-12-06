#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeType.h>

#include "CollisionShapeManager.h"
#include "physicsObject.h"

#include <xkill-utilities/EventManager.h>

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
	while(physicsObjects_->size() > 0)
	{
		physicsObjects_->at(physicsObjects_->size()-1)->removeFromWorld(dynamicsWorld_);
		delete physicsObjects_->at(physicsObjects_->size()-1);
		physicsObjects_->pop_back();
	}
	delete physicsObjects_;

	dynamicsWorld_->removeRigidBody(floor_);
	delete floor_;


    delete dynamicsWorld_;
    delete solver_;
    delete collisionConfiguration_;
    delete dispatcher_;
    delete broadphase_;
	collisionShapeManager_->clean();
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
	collisionShapeManager_ = CollisionShapeManager::getInstance();

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
		physicsObjects_->push_back(new PhysicsObject());
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
			physicsObject->preStep(physicsAttribute);
		}
		else if(physicsObject->isInWorld())
		{
			physicsObject->removeFromWorld(dynamicsWorld_);
		}
	}
	
	dynamicsWorld_->stepSimulation(delta,10);

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

