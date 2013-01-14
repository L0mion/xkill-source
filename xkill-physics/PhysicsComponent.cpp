#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/EventManager.h>

#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"
#include "physicsObject.h"
#include "collisionShapes.h"






AttributeIterator<Attribute_Physics> itrPhysics;
AttributeIterator<Attribute_Input> itrInput;

PhysicsComponent::PhysicsComponent() : broadphase_(nullptr),
									   collisionConfiguration_(nullptr),
									   dispatcher_(nullptr),
									   solver_(nullptr),
									   dynamicsWorld_(nullptr),
									   bulletImporter_(nullptr),
									   physicsObjects_(nullptr)
{
	SUBSCRIBE_TO_EVENT(this,EVENT_DO_CULLING);
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	itrInput = ATTRIBUTE_MANAGER->input.getIterator();
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
	delete CollisionShapes::Instance();
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

	dynamicsWorld_->setGravity(btVector3(0,-10,0));
	CollisionShapes::Instance()->loadCollisionShapes();
	

	return true;
}

void PhysicsComponent::handleInput()
{
	while(itrInput.hasNext())
	{
		Attribute_Input* inputAttribute = itrInput.getNext();
		physicsObjects_->at(inputAttribute->ptr_physics.index)->handleInput(inputAttribute);
	}
}

void PhysicsComponent::onUpdate(float delta)
{
	syncronizeWithAttributes();
	for(unsigned int i = 0; i < physicsObjects_->size(); i++)
	{
		physicsObjects_->at(i)->onUpdate(delta);
	}
	handleInput();
	dynamicsWorld_->stepSimulation(delta,10);
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
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	while(itrPhysics.hasNext())
	{
		Attribute_Physics* physicsAttribute = itrPhysics.getNext();
		unsigned int index = itrPhysics.index();
		if(index >= physicsObjects_->size())
		{
			physicsObjects_->push_back(nullptr);
		}
		if(itrPhysics.ownerId() == 0)
		{
			if(physicsObjects_->at(index) != nullptr)
			{
				dynamicsWorld_->removeRigidBody(physicsObjects_->at(index));
				delete physicsObjects_->at(index);
				physicsObjects_->at(index) = nullptr;
			}
		}
		if(physicsAttribute->hasChanged)
		{
			if(physicsObjects_->at(index) != nullptr)
			{
				dynamicsWorld_->removeRigidBody(physicsObjects_->at(index));
				delete physicsObjects_->at(index);
			}
			// Add object based on type
			/*switch(physicsAttribute->collisionFilterGroup())
			{
				DEFAULT_ERROR:*/
				physicsObjects_->at(index) = new PhysicsObject(index);
				/*break;
			}*/
			physicsObjects_->at(index)->init(index);
			dynamicsWorld_->addRigidBody(physicsObjects_->at(index));			
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