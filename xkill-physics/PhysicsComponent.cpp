#include "PhysicsComponent.h"

#include <btBulletDynamicsCommon.h>
#include "Serialize/BulletWorldImporter/btBulletWorldImporter.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"//check

#include <xkill-utilities/Util.h>

#include "PhysicsObject.h"
#include "PlayerPhysicsObject.h"
#include "ProjectilePhysicsObject.h"
#include "ExplosionSpherePhysicsObject.h"
#include "FrustumPhysicsObject.h"
#include "PickupablePhysicsObject.h"
#include "WorldPhysicsObject.h"
#include "physicsUtilities.h"

#include "CollisionShapes.h"
#include "debugDrawDispatcher.h"

ATTRIBUTES_DECLARE_ALL;

static debugDrawDispatcher gDebugDraw;

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
	dynamicsWorld_->setInternalTickCallback(wrapTickCallback,static_cast<void*>(this)); //Register collision callback

	gDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld_->setDebugDrawer(&gDebugDraw);

	CollisionShapes::Instance()->loadCollisionShapes();
	
	return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	gDebugDraw.clearDebugVerticesVector();

	//Loop through all physics attributes
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	while(itrPhysics.hasNext())
	{
		AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
		unsigned int physicsAttributeIndex = itrPhysics.storageIndex();

		synchronizeWithAttributes(ptr_physics, physicsAttributeIndex);	//Synchronize physics objects with physics attributes
		physicsObjects_->at(physicsAttributeIndex)->onUpdate(delta);	//Update physics objects by calling their onUpdate function.

		//Should be in PlayerPhysicsAttribute::onUpdate()
		if(ptr_physics->collisionFilterGroup == ptr_physics->PhysicsAttributeType::PLAYER)
		{
			//Calculate player aiming ray
			Entity* playerEntity = itrPhysics.ownerAt(physicsAttributeIndex);
			std::vector<int> rayttributeId = playerEntity->getAttributes(ATTRIBUTE_RAY);
			for(int i=0;i<rayttributeId.size();i++)
			{
				AttributePtr<Attribute_Ray> ray = itrRay.at(rayttributeId.at(i));
				btVector3 from = convert(ray->from);
				btVector3 to = convert(ray->to);
				gDebugDraw.drawLine(from,to,btVector4(1,1,1,1));
				btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
				allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
				dynamicsWorld_->rayTest(from,to,allResults);

				for (int i=0;i<allResults.m_hitFractions.size();i++)
				{
					btVector3 p = from.lerp(to,allResults.m_hitFractions[i]);
					gDebugDraw.drawSphere(p,0.1,btVector3(1.0f, 0.0f, 0.0f));
				}
			}
		}
	}


	//Ray cast
	/*
	btVector3 from(1,2,1);
	btVector3 to(1,-2,1);
	btCollisionWorld::ClosestRayResultCallback closestResults(from,to);
	dynamicsWorld_->rayTest(from,to,closestResults);
	if (closestResults.hasHit())
	{
		btVector3 p = from.lerp(to,closestResults.m_closestHitFraction);
		gDebugDraw.drawSphere(p,0.1,btVector3 (0,0,1));
		gDebugDraw.drawLine(p,p+closestResults.m_hitNormalWorld,btVector3 (0,0,1));
	}
	*/


	//dynamicsWorld_->updateAabbs();
	//dynamicsWorld_->computeOverlappingPairs();

	
	///all hits
	{
		btVector3 from(1,20,1);
		btVector3 to(1,-20,1);
		gDebugDraw.drawLine(from,to,btVector4(1,1,1,1));
		btCollisionWorld::AllHitsRayResultCallback allResults(from,to);
		allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
		dynamicsWorld_->rayTest(from,to,allResults);

		for (int i=0;i<allResults.m_hitFractions.size();i++)
		{
			btVector3 p = from.lerp(to,allResults.m_hitFractions[i]);
			gDebugDraw.drawSphere(p,0.1,btVector3(1.0f, 0.0f, 0.0f));
		}
	}

	updateCulling();

	dynamicsWorld_->stepSimulation(delta,0); //Bullet Physics physics simulation

	bool showDebug = ATTRIBUTE_MANAGER->settings->showDebugPhysics;
	if(showDebug)
	{
		dynamicsWorld_->debugDrawWorld(); //Calls debugDrawDispatcher::drawLine internally
		gDebugDraw.queueDebugDrawEvent();
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

		//Cast void pointer sent in Event_ModifyPhysicsObject, and modify physics object
		int physicsAttributeIndex = modifyPhysicsObject->ptr_physics.index();

		if(physicsAttributeIndex < physicsObjects_->size() && physicsAttributeIndex > -1)
		{
			if(physicsObjects_->at(physicsAttributeIndex) != NULL)
			{
				switch(modifyPhysicsObject->modifyWhatDataInPhysicsObjectData)
				{
				case XKILL_Enums::ModifyPhysicsObjectData::GRAVITY:
					{
						Float3* gravity = static_cast<Float3*>(modifyPhysicsObject->data);
						physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(gravity->x, gravity->y, gravity->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::VELOCITY:
					{
						Float3* velocity = static_cast<Float3*>(modifyPhysicsObject->data);
						physicsObjects_->at(physicsAttributeIndex)->setLinearVelocity(btVector3(velocity->x, velocity->y, velocity->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::VELOCITYPERCENTAGE:
					{
						Float3* velocityPercentage = static_cast<Float3*>(modifyPhysicsObject->data);
						btVector3 currentLinearVelocity = physicsObjects_->at(physicsAttributeIndex)->getLinearVelocity();
						physicsObjects_->at(physicsAttributeIndex)->setLinearVelocity(btVector3(currentLinearVelocity.x()*velocityPercentage->x, currentLinearVelocity.y()*velocityPercentage->y, currentLinearVelocity.z()*velocityPercentage->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::FLAG_STATIC:
					{
						bool* staticPhysicsObject = static_cast<bool*>(modifyPhysicsObject->data);
						if(*staticPhysicsObject == true)
						{
							physicsObjects_->at(physicsAttributeIndex)->setCollisionFlags(physicsObjects_->at(physicsAttributeIndex)->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
						}
						else if(*staticPhysicsObject == false)
						{
							physicsObjects_->at(physicsAttributeIndex)->setCollisionFlags(physicsObjects_->at(physicsAttributeIndex)->getCollisionFlags() & ~ btCollisionObject::CF_STATIC_OBJECT);
						}
						break;
					}
				}
			}
			else
			{
				DEBUGPRINT("Invalid physics attribute id when handling event of type EVENT_MODIFY_PHYSICS_OBJECT, error 1");
			}
		}
		else
		{
			DEBUGPRINT("Invalid physics attribute id when handling event of type EVENT_MODIFY_PHYSICS_OBJECT, error 2");
		}
		break;
	}
	//case EVENT_LOAD_LEVEL:
	//	break;
	}
}

void PhysicsComponent::synchronizeWithAttributes(AttributePtr<Attribute_Physics> ptr_physics, int physicsAttributeIndex)
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
	if(physicsAttributeIndex >= physicsObjects_->size())
	{
		physicsObjects_->push_back(nullptr);
	}
	//Synchronize physiscs attributes with internal PhysicsObjects
	if(ptr_physics->reloadDataIntoBulletPhysics) //If something has changed in the physics attribute
	{
		//If the PhysicsObjects already exists, it needs to be removed to safely reset all of its internal Bullet Physics values
		if(physicsObjects_->at(physicsAttributeIndex) != nullptr)
		{
			dynamicsWorld_->removeRigidBody(physicsObjects_->at(physicsAttributeIndex));
			delete physicsObjects_->at(physicsAttributeIndex);
		}
		// Determine type of PhysicsObject to create and add to the Bullet Physics world
		switch(ptr_physics->collisionFilterGroup)
		{
		default:
			physicsObjects_->at(physicsAttributeIndex) = new PhysicsObject();
			break;
		case Attribute_Physics::WORLD:
			physicsObjects_->at(physicsAttributeIndex) = new WorldPhysicsObject();
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
			SHOW_MESSAGEBOX("Error: Attribute_Physics should not have EVERYTHING as collisionFilterGroup");
			break;
		}

		if(physicsObjects_ != nullptr)
		{
			if(physicsObjects_->at(physicsAttributeIndex)->init(physicsAttributeIndex, ptr_physics->collisionFilterGroup) == true)
			{
				dynamicsWorld_->addRigidBody(physicsObjects_->at(physicsAttributeIndex), ptr_physics->collisionFilterGroup, ptr_physics->collisionFilterMask);
				//Per object gravity must be set after "addRigidBody"
				if(!physicsObjects_->at(physicsAttributeIndex)->isStaticOrKinematicObject())
				{
					physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(ptr_physics->gravity.x, ptr_physics->gravity.y, ptr_physics->gravity.z));
					//physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(0,0,0));
				}

				ptr_physics->reloadDataIntoBulletPhysics = false;
			}
			else
			{
				SHOW_MESSAGEBOX("-->Error initializing PhysicsObject");
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
	itrPhysics.at(objectAttributeIndex)->ptr_render->cull = true;
}

void PhysicsComponent::updateCulling()
{
	while(itrPhysics.hasNext())
	{
		AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
		if(ptr_physics->ptr_render.isNotEmpty())
		{
			ptr_physics->ptr_render->cull = false;
		}
	}
	CollisionShapes::Instance()->updateFrustrumShape();

	while(itrCamera.hasNext())
	{
		AttributePtr<Attribute_Camera> ptr_camera = itrCamera.getNext();
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