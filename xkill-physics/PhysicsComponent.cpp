#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>
#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"

#include <xkill-utilities/EventManager.h>

#include "PhysicsObject.h"
#include "PlayerPhysicsObject.h"
#include "ProjectilePhysicsObject.h"
#include "ExplosionSpherePhysicsObject.h"
#include "FrustumPhysicsObject.h"
#include "PickupablePhysicsObject.h"
#include "physicsUtilities.h"

#include "CollisionShapes.h"
#include "debugDrawDispatcher.h"

#include <iostream>
ATTRIBUTES_DECLARE_ALL;

static debugDrawDispatcher gDebugDraw;
static float removePhysicsObjectIfItHasLowerYCoordinateThanThis;

PhysicsComponent::PhysicsComponent() : broadphase_(nullptr),
									   collisionConfiguration_(nullptr),
									   dispatcher_(nullptr),
									   solver_(nullptr),
									   dynamicsWorld_(nullptr),
									   bulletImporter_(nullptr),
									   physicsObjects_(nullptr)
{
	ATTRIBUTES_INIT_ALL;
	SUBSCRIBE_TO_EVENT(this,EVENT_ATTRIBUTE_UPDATED);
	SUBSCRIBE_TO_EVENT(this, EVENT_MODIFY_PHYSICS_OBJECT);
	removePhysicsObjectIfItHasLowerYCoordinateThanThis = -5.0f;
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

	gDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld_->setDebugDrawer(&gDebugDraw);

	CollisionShapes::Instance()->loadCollisionShapes();
	
	return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	//Update physics objects, which are synchronized with physics attributes
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	while(itrPhysics.hasNext())
	{
		Attribute_Physics* physicsAttribute = itrPhysics.getNext();
		unsigned int index = itrPhysics.storageIndex();

		synchronizeWithAttributes(physicsAttribute, index);
		physicsObjects_->at(index)->onUpdate(delta);

		//Physics object out of bounds
		if(physicsObjects_->at(index)->getWorldTransform().getOrigin().y() < removePhysicsObjectIfItHasLowerYCoordinateThanThis)
		{
			//Player out of bounds
			if(physicsAttribute->collisionFilterGroup == Attribute_Physics::PLAYER)
			{
				int playerEntityIndex = itrPhysics.ownerIdAt(index);
				Entity* playerEntity = itrPhysics.ownerAt(index);
				
				std::vector<int> playerAttributeIndices = playerEntity->getAttributes(ATTRIBUTE_PLAYER);
				for(unsigned int i = 0; i < playerAttributeIndices.size(); i++)
				{
					Attribute_Player* playerAttribute = itrPlayer.at(i);
					Attribute_Health* playerHealthAttribute = itrHealth.at(playerAttribute->ptr_health);
					if(playerHealthAttribute->health > 0.0f)
					{
						DEBUGPRINT("Player entity " << playerEntityIndex << " was out of bounds");
						SEND_EVENT(&Event_PlayerDeath(playerAttributeIndices[i]));
					}
				}
			}
			else
			{
				btTransform transform;
				btVector3 newPosition = btVector3(0.0f, 10.0f, 0.0f);

				transform = physicsObjects_->at(index)->getWorldTransform();
				transform.setOrigin(newPosition);
				physicsObjects_->at(index)->setWorldTransform(transform);

				DEBUGPRINT("Something that was out of bounds was moved to " << newPosition.x() << " " << newPosition.y() << " " << newPosition.z());
			}
		}
	}

	updateCulling();

	dynamicsWorld_->stepSimulation(delta,0); //Bullet Physics physics simulation

	bool showDebug =  ATTRIBUTE_MANAGER->settings->showDebugPhysics;
	if(showDebug)
	{
		//static float timer = 0.0f;
		//if(timer > 0.1f)
		{
			gDebugDraw.clearDebugVerticesVector();
			dynamicsWorld_->debugDrawWorld();
			gDebugDraw.queueDebugDrawEvent();
			//timer = 0.0f;
		}
		//timer += delta;
	}

	FLUSH_QUEUED_EVENTS(EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
}

void PhysicsComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch(type)
	{
	case EVENT_ATTRIBUTE_UPDATED: //Removes physics objects when the corresponding physics attribute is removed
	{
		Event_AttributeUpdated* attributeUpdated = static_cast<Event_AttributeUpdated*>(e);
		int attributeIndex = attributeUpdated->index;
		if(attributeUpdated->attributeEnum == ATTRIBUTE_PHYSICS)
		{
			if(attributeUpdated->isDeleted)
			{
				if(attributeIndex < physicsObjects_->size() && physicsObjects_->at(attributeIndex) != nullptr)
				{
  					dynamicsWorld_->removeRigidBody(physicsObjects_->at(attributeIndex));
					delete physicsObjects_->at(attributeIndex);
					physicsObjects_->at(attributeIndex) = nullptr;
				}
				else
				{
					DEBUGPRINT("Mismatch when synchronizing deletion of physics objects with physics attributes");
				}
			}
			else if(attributeUpdated->isCreated)
			{
			}
			else
			{
				itrPhysics.at(attributeIndex)->reloadDataIntoBulletPhysics = true;
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
	}
	case EVENT_MODIFY_PHYSICS_OBJECT:
	{
		Event_ModifyPhysicsObject* modifyPhysicsObject = static_cast<Event_ModifyPhysicsObject*>(e);

		int physicsAttributeIndex = modifyPhysicsObject->physicsAttributeIndex;
		if(physicsAttributeIndex < physicsObjects_->size())
		{
			switch(modifyPhysicsObject->modifyWhatDataInPhysicsObjectData)
			{
			case GRAVITY:
				Float3* gravity = static_cast<Float3*>(modifyPhysicsObject->data);
				physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(gravity->x, gravity->y, gravity->z));
				break;
			}
		}
		else
		{
			DEBUGPRINT("Invalid physics attribute id when handling event of type EVENT_MODIFY_PHYSICS_OBJECT");
		}
		break;
	}
	//case EVENT_LOAD_LEVEL:
	//	break;
	}
}

void PhysicsComponent::synchronizeWithAttributes(Attribute_Physics* physicsAttribute, int physicsAttributeIndex)
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
	//Checks if new physiscs attributes were created since last call to this function
	if(physicsAttributeIndex >= static_cast<unsigned int>(physicsObjects_->size()))
	{
		physicsObjects_->push_back(nullptr);
	}
	//Synchronize physiscs attributes with internal PhysicsObjects
	if(physicsAttribute->reloadDataIntoBulletPhysics) //If something has changed in the physics attribute
	{
		//If the PhysicsObjects already exists, it needs to be removed to safely reset all of its internal Bullet Physics values
		if(physicsObjects_->at(physicsAttributeIndex) != nullptr)
		{
			dynamicsWorld_->removeRigidBody(physicsObjects_->at(physicsAttributeIndex));
			delete physicsObjects_->at(physicsAttributeIndex);
		}
		// Determine type of PhysicsObject to create and add to the Bullet Physics world
		switch(physicsAttribute->collisionFilterGroup)
		{
		default:
			physicsObjects_->at(physicsAttributeIndex) = new PhysicsObject();
			break;
		case Attribute_Physics::WORLD:
			physicsObjects_->at(physicsAttributeIndex) = new PhysicsObject();
			break;
		case Attribute_Physics::PLAYER:
			physicsObjects_->at(physicsAttributeIndex) = new PlayerPhysicsObject();
			break;
		case Attribute_Physics::PROJECTILE:
			physicsObjects_->at(physicsAttributeIndex) = new ProjectilePhysicsObject();
			break;
		case Attribute_Physics::EXPLOSIONSPHERE:
			physicsObjects_->at(physicsAttributeIndex) = new ExplosionSpherePhysicsObject();
			break;
		case Attribute_Physics::PICKUPABLE:
			physicsObjects_->at(physicsAttributeIndex) = new PickupablePhysicsObject();
			break;
		case Attribute_Physics::EVERYTHING:
			std::cout << "Error: Attribute_Physics should not have EVERYTHING as collisionFilterGroup" << std::endl;
			break;
		}

		if(physicsObjects_ != nullptr)
		{
			if(physicsObjects_->at(physicsAttributeIndex)->init(physicsAttributeIndex,physicsAttribute->collisionFilterGroup) == true)
			{
				dynamicsWorld_->addRigidBody(physicsObjects_->at(physicsAttributeIndex), physicsAttribute->collisionFilterGroup, physicsAttribute->collisionFilterMask);
				//Per object gravity must be set after "addRigidBody"
				if(!physicsObjects_->at(physicsAttributeIndex)->isStaticOrKinematicObject())
				{
					physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(physicsAttribute->gravity.x,physicsAttribute->gravity.y, physicsAttribute->gravity.z));
					//physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(0,0,0));
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

				//check physicsobjecttype;
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

	//itrRender.at(itrPhysics.at(objectAttributeIndex)->ptr_render)->culling.setBool(frustumAttributeIndex,true);
	itrRender.at(itrPhysics.at(objectAttributeIndex)->ptr_render)->cull = true;
}

void PhysicsComponent::updateCulling()
{
	static int testvar = 0;
	if(testvar < 1)
	{
		testvar++;
		return;
	}
	while(itrRender.hasNext())
	{
		Attribute_Render * ra = itrRender.getNext();
		//ra->culling.clear();
		ra->cull = false;
		int a =0;
	}
	CollisionShapes::Instance()->updateFrustrumShape();

	while(itrCamera.hasNext())
	{
		Attribute_Camera* cameraAttribute = itrCamera.getNext();
		unsigned int index = itrCamera.storageIndex();
		
		if(index >= static_cast<unsigned int>(frustumPhysicsObjects_->size()))
		{
			frustumPhysicsObjects_->push_back(nullptr);
		}
		if(frustumPhysicsObjects_->at(index) == nullptr)
		{
			frustumPhysicsObjects_->at(index) = new FrustumPhysicsObject();
			frustumPhysicsObjects_->at(index)->frustumInit(index,Attribute_Physics::FRUSTUM);
			int a = dynamicsWorld_->getNumCollisionObjects();
			dynamicsWorld_->addRigidBody(frustumPhysicsObjects_->at(index),Attribute_Physics::FRUSTUM,Attribute_Physics::EVERYTHING);
			frustumPhysicsObjects_->at(index)->setGravity(btVector3(0,0,0));
			int b = dynamicsWorld_->getNumCollisionObjects();
			if(a==b)
			{
				int bad = 2;
			}
		}
		else
		{
			frustumPhysicsObjects_->at(index)->onUpdate(0);
		}
	}
}