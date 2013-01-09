#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"
#include "physicsObject.h"

PhysicsComponent::PhysicsComponent() : broadphase_(nullptr),
									   collisionConfiguration_(nullptr),
									   dispatcher_(nullptr),
									   solver_(nullptr),
									   dynamicsWorld_(nullptr),
									   bulletImporter_(nullptr),
									   collisionObjects_(nullptr),
									   physicsObjects_(nullptr),
									   collisionShapes_(nullptr)
{
	SUBSCRIBE_TO_EVENT(this,EVENT_DO_CULLING);
}

PhysicsComponent::~PhysicsComponent()
{
	// Remove all PhysicsObjects
	if(physicsObjects_ != nullptr)
	{
		while(physicsObjects_->size() > 0)
		{
			unsigned int last = physicsObjects_->size()-1;
			if(physicsObjects_->at(last) != nullptr)
			{
				dynamicsWorld_->removeRigidBody(physicsObjects_->at(last));
				delete physicsObjects_->at(last);
			}
			physicsObjects_->pop_back();
		}
		delete physicsObjects_;
	}
	// Remove all CollisionObjects
	if(collisionObjects_ != nullptr)
	{
		while(collisionObjects_->size() > 0)
		{
			unsigned int last = collisionObjects_->size()-1;
			if(collisionObjects_->at(last) != nullptr)
			{
				//dynamicsWorld_->removeCollisionObject(collisionObjects_->at(last)); // ADD WHEN COLLISIONOBJECTS IS DONE
				delete collisionObjects_->at(last);
			}
			collisionObjects_->pop_back();
		}
		delete collisionObjects_;
	}
	// Remove all btCollisionShapes
	if(collisionShapes_ != nullptr)
	{
		while(collisionShapes_->size() > 0)
		{
			unsigned int last = collisionShapes_->size()-1;
			if(collisionShapes_->at(last) != nullptr)
			{
				delete collisionShapes_->at(last);
			}
			collisionShapes_->pop_back();
		}
		delete collisionShapes_;
	}
	// Remove the btCollisionDispatcher
	if(dispatcher_ != nullptr)
	{
		delete dispatcher_;
	}
	// Remove the btBroadphaseInterface
	if(broadphase_ != nullptr)
	{
		delete broadphase_;
	}
	// Remove the btSequentialImpulseConstraintSolver
	if(solver_ != nullptr)
	{
		delete solver_;
	}
	// Remove the btDefaultCollisionConfiguration
	if(collisionConfiguration_ != nullptr)
	{
		delete collisionConfiguration_;
	}
	// Remove the btDynamicWorld
	if(dynamicsWorld_ != nullptr)
	{
		delete dynamicsWorld_;
	}
	// Remove the btBulletWorldImporter
	if(bulletImporter_ != nullptr)
	{
		delete bulletImporter_;
	}
}

bool PhysicsComponent::init()
{
	physicsObjects_ =	new btAlignedObjectArray<PhysicsObject*>();
	collisionObjects_ = new btAlignedObjectArray<CollisionObject*>();
	collisionShapes_ =	new btAlignedObjectArray<btCollisionShape*>();

	
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	solver_ =				  new btSequentialImpulseConstraintSolver();
	broadphase_ =			  new btDbvtBroadphase();
	dispatcher_	=			  new btCollisionDispatcher(collisionConfiguration_);

	dynamicsWorld_ =  new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
	bulletImporter_ = new btBulletWorldImporter(dynamicsWorld_);

	dynamicsWorld_->setGravity(btVector3(0,0,0));

	return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	syncronizeWithAttributes();
	preStep();
	dynamicsWorld_->stepSimulation(delta);
	postStep();
}

void PhysicsComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_DO_CULLING:
		doCulling();
		break;
	//case EVENT_LOAD_LEVEL:
	//	break;
	}
}

void loadObjects()
{

}

void PhysicsComponent::syncronizeWithAttributes()
{
}

void PhysicsComponent::preStep()
{
}

void PhysicsComponent::postStep()
{
}

void PhysicsComponent::collisionDetection()
{
}

void PhysicsComponent::doCulling()
{
}