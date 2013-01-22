#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>
#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"

#include <xkill-utilities/EventManager.h>

#include "PhysicsObject.h"
#include "PlayerPhysicsObject.h"
#include "ProjectilePhysicsObject.h"
#include "ExplosionSpherePhysicsObject.h"
#include "FrustumPhysicsObject.h"
#include "physicsUtilities.h"

#include "CollisionShapes.h"

#include <iostream>

AttributeIterator<Attribute_Physics> itrPhysics;
AttributeIterator<Attribute_Render> itrRender;
AttributeIterator<Attribute_Camera> itrCamera_2;
PhysicsComponent::PhysicsComponent() : broadphase_(nullptr),
									   collisionConfiguration_(nullptr),
									   dispatcher_(nullptr),
									   solver_(nullptr),
									   dynamicsWorld_(nullptr),
									   bulletImporter_(nullptr),
									   physicsObjects_(nullptr)
{
	SUBSCRIBE_TO_EVENT(this,EVENT_DO_CULLING);
	SUBSCRIBE_TO_EVENT(this,EVENT_ATTRIBUTE_UPDATED);
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	itrRender = ATTRIBUTE_MANAGER->render.getIterator();
	itrCamera_2 = ATTRIBUTE_MANAGER->camera.getIterator();
}

PhysicsComponent::~PhysicsComponent()
{
	UNSUBSCRIBE_TO_EVENTS(this);
	// Remove all PhysicsObjects
	if(frustumPhysicsObjects_ != nullptr)
	{
		while(frustumPhysicsObjects_->size() > 0)
		{
			if(frustumPhysicsObjects_->at(frustumPhysicsObjects_->size() - 1) != nullptr)
			{
				dynamicsWorld_->removeRigidBody(frustumPhysicsObjects_->at(frustumPhysicsObjects_->size() - 1));
				delete frustumPhysicsObjects_->at(frustumPhysicsObjects_->size() - 1);
			}
			frustumPhysicsObjects_->pop_back();
		}
		delete frustumPhysicsObjects_;
	}
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
	frustumPhysicsObjects_ = new btAlignedObjectArray<FrustumPhysicsObject*>();
	
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	solver_ =				  new btSequentialImpulseConstraintSolver();
	broadphase_ =			  new btDbvtBroadphase();
	dispatcher_	=			  new btCollisionDispatcher(collisionConfiguration_);

	dynamicsWorld_ =  new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
	bulletImporter_ = new btBulletWorldImporter(dynamicsWorld_);

	dynamicsWorld_->setGravity(btVector3(0,-10,0));
	dynamicsWorld_->setInternalTickCallback(wrapTickCallback,static_cast<void*>(this));

	CollisionShapes::Instance()->loadCollisionShapes();
	
	return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	synchronizeWithAttributes();
	for(unsigned int i = 0; i < static_cast<unsigned int>(physicsObjects_->size()); i++)
	{
		if(physicsObjects_->at(i) != nullptr)
		{
			physicsObjects_->at(i)->onUpdate(delta);
		}
	}
	updateCulling();
	dynamicsWorld_->stepSimulation(delta,0);
	FLUSH_QUEUED_EVENTS(EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
}

void PhysicsComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch(type)
	{
	case EVENT_DO_CULLING:
		updateCulling();
		break;
	case EVENT_ATTRIBUTE_UPDATED: //Removes physics objects when the corresponding physics attribute is removed
		Event_AttributeUpdated* attributeUpdated = static_cast<Event_AttributeUpdated*>(e);
		int attributeIndex = attributeUpdated->index;
		if(attributeUpdated->attributeEnum == ATTRIBUTE_PHYSICS)
		{
			if(attributeUpdated->isDeleted)
			{
  				dynamicsWorld_->removeRigidBody(physicsObjects_->at(attributeIndex));
				delete physicsObjects_->at(attributeIndex);
				physicsObjects_->at(attributeIndex) = nullptr;
			}
		}
		else if(attributeUpdated->attributeEnum == ATTRIBUTE_CAMERA)
		{
			if(attributeUpdated->isDeleted)
			{
  				dynamicsWorld_->removeRigidBody(frustumPhysicsObjects_->at(attributeIndex));
				delete frustumPhysicsObjects_->at(attributeIndex);
				frustumPhysicsObjects_->at(attributeIndex) = nullptr;
			}
		}
		break;
	//case EVENT_LOAD_LEVEL:
	//	break;
	}
}

void PhysicsComponent::synchronizeWithAttributes()
{
	//Also refer to PhysicsComponent::onEvent, handling of EVENT_ATTRIBUTE_UPDATED
	
	//Old physics attribute <--> physics object remove synchronization
	/*
	for(int i = 0; i < itrPhysics.storageSize(); i++)
	{
		if( itrPhysics.ownerIdAt(i) == 0 && physicsObjects_->at(i) != nullptr)
		{
			dynamicsWorld_->removeRigidBody(physicsObjects_->at(i));
			delete physicsObjects_->at(i);
			physicsObjects_->at(i) = nullptr;
		}
	}
	*/
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	while(itrPhysics.hasNext())
	{
		Attribute_Physics* physicsAttribute = itrPhysics.getNext();
		unsigned int index = itrPhysics.storageIndex();
		
		//Checks if new physiscs attributes were created since last call to this function
		if(index >= static_cast<unsigned int>(physicsObjects_->size()))
		{
			physicsObjects_->push_back(nullptr);
		}
		//Synchronize physiscs attributes with internal PhysicsObjects
		if(physicsAttribute->reloadDataIntoBulletPhysics) //If something has changed in the physics attribute
		{
			//If the PhysicsObjects already exists, it needs to be removed to safely reset all of its internal Bullet Physics values
			if(physicsObjects_->at(index) != nullptr)
			{
				dynamicsWorld_->removeRigidBody(physicsObjects_->at(index));
				delete physicsObjects_->at(index);
			}
			// Determine type of PhysicsObject to create and add to the Bullet Physics world
			switch(physicsAttribute->collisionFilterGroup)
			{
			case Attribute_Physics::DEFAULT_ERROR:
				std::cout << "Error: Attribute_Physics should not have be DEFAULT_ERROR as collisionFilterGroup. Do not forget to set collisionFilterGroup when creating Attribute_Physics." << std::endl;
				break;
			case Attribute_Physics::WORLD:
				physicsObjects_->at(index) = new PhysicsObject();
				break;
			case Attribute_Physics::PLAYER:
				physicsObjects_->at(index) = new PlayerPhysicsObject();
				break;
			case Attribute_Physics::PROJECTILE:
				physicsObjects_->at(index) = new ProjectilePhysicsObject();
				break;
			case Attribute_Physics::EXPLOSIONSPHERE:
				physicsObjects_->at(index) = new ExplosionSpherePhysicsObject();
				break;
			case Attribute_Physics::EVERYTHING:
				std::cout << "Error: Attribute_Physics should not have EVERYTHING as collisionFilterGroup" << std::endl;
				break;
			}

			if(physicsObjects_ != nullptr)
			{
				if(physicsObjects_->at(index)->init(index,physicsAttribute->collisionFilterGroup) == true)
				{
					dynamicsWorld_->addRigidBody(physicsObjects_->at(index), physicsAttribute->collisionFilterGroup, physicsAttribute->collisionFilterMask);
					//Per object gravity must be set after "addRigidBody"
					if(!physicsObjects_->at(index)->isStaticOrKinematicObject())
					{
						physicsObjects_->at(index)->setGravity(btVector3(physicsAttribute->gravity.x,physicsAttribute->gravity.y, physicsAttribute->gravity.z));
					}

					physicsAttribute->reloadDataIntoBulletPhysics = false;
				}
				else
				{
					std::cout << "-->Error initializing PhysicsObject" << std::endl;
				}
			}
		}
	}
}

void wrapTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    PhysicsComponent *component = static_cast<PhysicsComponent *>(world->getWorldUserInfo());
	component->detectedCollisionsDuringStepSimulation(timeStep);
}

void PhysicsComponent::detectedCollisionsDuringStepSimulation(btScalar timeStep)
{
	btPersistentManifold* persistentManifold;
	btDispatcher* dispatcher = dynamicsWorld_->getDispatcher();
	unsigned int numManifolds = dynamicsWorld_->getDispatcher()->getNumManifolds();
	
	// loop through all manifolds from last timestep
	for(unsigned int i = 0; i < numManifolds; i++)
	{
		persistentManifold = dispatcher_->getManifoldByIndexInternal(i);
		unsigned int numContacts = persistentManifold->getNumContacts();
		
		// loop through all contact points of manifold
		for(unsigned int j = 0; j < numContacts; j++)
		{
			// ignore contacts where distance is larger than 0
			if(persistentManifold->getContactPoint(j).getDistance()<0.0f)
			{
				const PhysicsObject* objectA = static_cast<const PhysicsObject*>(persistentManifold->getBody0());
				const PhysicsObject* objectB = static_cast<const PhysicsObject*>(persistentManifold->getBody1());
				
				unsigned int ownerA = itrPhysics.ownerIdAt(objectA->getAttributeIndex());
				unsigned int ownerB = itrPhysics.ownerIdAt(objectB->getAttributeIndex());
				
				//////check physicsobjecttype;
				if(objectA->getCollisionFilterGroup() == Attribute_Physics::FRUSTUM ||
					objectB->getCollisionFilterGroup() == Attribute_Physics::FRUSTUM)
				{
					if(objectA->getCollisionFilterGroup() == Attribute_Physics::FRUSTUM &&
					   objectB->getCollisionFilterGroup() != Attribute_Physics::FRUSTUM)
					{
						doCulling(objectA->getAttributeIndex(),objectB->getAttributeIndex());
					}
					else if(objectA->getCollisionFilterGroup() != Attribute_Physics::FRUSTUM &&
					   objectB->getCollisionFilterGroup() == Attribute_Physics::FRUSTUM)
					{
						doCulling(objectB->getAttributeIndex(),objectA->getAttributeIndex());
					}					
				}
				else
				{
					//std::cout << "\nCollision between " << ownerA << " & " << ownerB;
					QUEUE_EVENT(new Event_PhysicsAttributesColliding(objectA->getAttributeIndex(), objectB->getAttributeIndex()));
					QUEUE_EVENT(new Event_PhysicsAttributesColliding(objectB->getAttributeIndex(), objectA->getAttributeIndex()));
					break;
				}
			}
		}
	}
}

void PhysicsComponent::doCulling(unsigned int frustumAttributeIndex, unsigned int objectAttributeIndex)
{
	itrRender.at(itrPhysics.at(objectAttributeIndex)->ptr_render)->culling.setBool(frustumAttributeIndex,true);
}

void PhysicsComponent::updateCulling()
{
	CollisionShapes::Instance()->updateFrustrumShape();

	itrCamera_2 = ATTRIBUTE_MANAGER->camera.getIterator();
	while(itrCamera_2.hasNext())
	{
		Attribute_Camera* cameraAttribute = itrCamera_2.getNext();
		unsigned int index = itrCamera_2.storageIndex();
		
		//Checks if new physiscs attributes were created since last call to this function
		if(index >= static_cast<unsigned int>(frustumPhysicsObjects_->size()))
		{
			frustumPhysicsObjects_->push_back(nullptr);
		}
		if(frustumPhysicsObjects_->at(index) == nullptr)
		{
			frustumPhysicsObjects_->at(index) = new FrustumPhysicsObject();
			frustumPhysicsObjects_->at(index)->init(index,Attribute_Physics::FRUSTUM);
			int a = dynamicsWorld_->getNumCollisionObjects();
			dynamicsWorld_->addRigidBody(frustumPhysicsObjects_->at(index),Attribute_Physics::FRUSTUM,Attribute_Physics::EVERYTHING);
			int b = dynamicsWorld_->getNumCollisionObjects();
		}
		else
		{
			frustumPhysicsObjects_->at(index)->onUpdate(0);
		}
	}
}