#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"
#include "physicsObject.h"

ATTRIBUTES_DECLARE_ALL;

PhysicsComponent::PhysicsComponent() : broadphase_(nullptr),
									   collisionConfiguration_(nullptr),
									   dispatcher_(nullptr),
									   solver_(nullptr),
									   dynamicsWorld_(nullptr),
									   bulletImporter_(nullptr),
									   physicsObjects_(nullptr)
{
	SUBSCRIBE_TO_EVENT(this,EVENT_DO_CULLING);
	ATTRIBUTES_INIT_ALL;
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
	dynamicsWorld_->stepSimulation(delta);
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

void PhysicsComponent::syncronizeWithAttributes()
{
	while(itrPhysics.hasNext())
	{
		Attribute_Physics* physicsAttribute = itrPhysics.getNext();
		unsigned int index = itrPhysics.index();
		if(index > physicsObjects_->size())
		{
			physicsObjects_->push_back(nullptr);
		}
		if(itrPhysics.ownerId() == 0)
		{
			if(physicsObjects_->at(index) != nullptr)
			{
				delete physicsObjects_->at(index);
				physicsObjects_->at(index) = nullptr;
			}
		}
		if(physicsAttribute->hasChanged)
		{
			if(physicsObjects_->at(index) != nullptr)
			{
				delete physicsObjects_->at(index);
			}
			// Add object based on type
			/*switch(physicsAttribute->collisionFilterGroup())
			{
				DEFAULT_ERROR:
				physicsObjects_->at(index) = new PhysicsObject();
				break;
			}*/
			physicsObjects_->at(index)->init(index);
			physicsAttribute->hasChanged = false;
		}
	}
}

void PhysicsComponent::collisionDetection()
{
	//Weehaa!
}

void PhysicsComponent::doCulling()
{
	//YippiKayey
}