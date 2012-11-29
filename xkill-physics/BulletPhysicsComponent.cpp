#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeType.h>

#include "physicsObject.h"

BulletPhysicsComponent::BulletPhysicsComponent()
{
	broadphase_ = nullptr;
	collisionConfiguration_ = nullptr;
	dispatcher_ = nullptr;
	solver_ = nullptr;
	dynamicsWorld_ = nullptr;
	physicsAttributes_ = nullptr;
	physicsObjects_ = nullptr;
	numPhysicsAttributes_ = 0;
}

BulletPhysicsComponent::~BulletPhysicsComponent()
{
	delete physicsObjects_;
	//delete local objects etc.
 
    delete dynamicsWorld_;
    delete solver_;
    delete collisionConfiguration_;
    delete dispatcher_;
    delete broadphase_;
}

bool BulletPhysicsComponent::Init(std::vector<PhysicsAttribute>* physicsAttributes)
{
		physicsAttributes_ = physicsAttributes; 
		physicsObjects_ = new btAlignedObjectArray<PhysicsObject*>();
		
        broadphase_ = new btDbvtBroadphase();
        collisionConfiguration_ = new btDefaultCollisionConfiguration();
        dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
        solver_ = new btSequentialImpulseConstraintSolver;
        
		dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
        dynamicsWorld_->setGravity(btVector3(0,-10,0));

        return true;
}

void BulletPhysicsComponent::onUpdate(float delta)
{
	for(unsigned int i = (*physicsObjects_).size(); i < numPhysicsAttributes_; i++)
	{
		physicsObjects_->push_back(new PhysicsObject());
	}
	for(unsigned int i = 0; i < numPhysicsAttributes_; i++)
	{
		if((*physicsAttributes_)[i].alive)
		{
			if((*physicsAttributes_)[i].added)
			{
				(*physicsObjects_)[i]->preStep(&(*physicsAttributes_)[i]);
			}
			else
			{
				(*physicsObjects_)[i]->Init(&(*physicsAttributes_)[i],dynamicsWorld_);
				(*physicsAttributes_)[i].added = true;
			}
		}
		else if((*physicsAttributes_)[i].added)
		{
			(*physicsObjects_)[i]->Clean(dynamicsWorld_);
			(*physicsAttributes_)[i].added = false;
		}
	}

	dynamicsWorld_->stepSimulation(delta,10);

	for(unsigned int i = 0; i < numPhysicsAttributes_; i++)
	{
		if((*physicsAttributes_)[i].alive && (*physicsAttributes_)[i].added)
		{
			(*physicsObjects_)[i]->postStep(&(*physicsAttributes_)[i]);
		}
	}
}

void BulletPhysicsComponent::onEvent(Event* e)
{

}