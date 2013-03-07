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
#include "PropPhysicsObject.h"
#include "physicsUtilities.h"

#include "debugDrawDispatcher.h"
#include "CollisionShapes.h"

#include <algorithm> //std::sort

ATTRIBUTES_DECLARE_ALL;

debugDrawDispatcher* debugDrawer_ = nullptr;

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
	SUBSCRIBE_TO_EVENT(this, EVENT_CLOSEST_HIT_RAY_CAST);
	SUBSCRIBE_TO_EVENT(this, EVENT_ALL_HITS_RAY_CAST);
	SUBSCRIBE_TO_EVENT(this, EVENT_UNLOAD_LEVEL);
	SUBSCRIBE_TO_EVENT(this, EVENT_LOAD_LEVEL_BULLET);
	SUBSCRIBE_TO_EVENT(this, EVENT_NULL_PROCESS_STOPPED_EXECUTING);
	SUBSCRIBE_TO_EVENT(this, EVENT_RELOAD_PHYSICS_ATTRIBUTE_DATA_INTO_BULLET_PHYSICS);
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

	if(debugDrawer_ != nullptr)
	{
		delete debugDrawer_;
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
	
	debugDrawer_ = new debugDrawDispatcher();
	debugDrawer_->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld_->setDebugDrawer(debugDrawer_);
	
	PhysicsObject::setDynamicsWorld(dynamicsWorld_); //Make dynamicsWorld_ accessible from physics objects
	PhysicsObject::setDebugDrawer(debugDrawer_);

	//CollisionShapes::Instance()->loadCollisionShapes();
	
	return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	debugDrawer_->clearDebugVerticesVector();

	//Loop through all physics attributes
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	while(itrPhysics.hasNext())
	{
		AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
		unsigned int physicsAttributeIndex = itrPhysics.storageIndex();

		synchronizeWithAttributes(ptr_physics, physicsAttributeIndex);	//Synchronize physics objects with physics attributes
		physicsObjects_->at(physicsAttributeIndex)->onUpdate(delta);	//Update physics objects by calling their onUpdate function.
	}

	//updateCulling();

	dynamicsWorld_->stepSimulation(delta, 0); //Bullet Physics physics simulation

	bool showDebug = SETTINGS->showDebugPhysics;
	if(showDebug)
	{
		for (int i=dynamicsWorld_->getNumCollisionObjects()-1; i>=0 ;i--)
		{
			//Get motion state world transform and store it as an internal world transform, which is the position used by "debugDrawWorld"
			btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform world;
			if(body->getMotionState())
			{
				body->getMotionState()->setWorldTransform(body->getWorldTransform());
				/*worldTransforms[i] = world;
				body->setWorldTransform(world);*/
			}
		}
		dynamicsWorld_->debugDrawWorld(); //Calls debugDrawDispatcher::drawLine internally
		debugDrawer_->queueDebugDrawEvent();
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
  				//dynamicsWorld_->removeRigidBody(frustumPhysicsObjects_->at(attributeIndex));
				//delete frustumPhysicsObjects_->at(attributeIndex);
				//frustumPhysicsObjects_->at(attributeIndex) = nullptr;
			}
		}
		break;
	}
	case EVENT_MODIFY_PHYSICS_OBJECT:
	{
		Event_ModifyPhysicsObject* modifyPhysicsObject = static_cast<Event_ModifyPhysicsObject*>(e);

		int physicsAttributeIndex = modifyPhysicsObject->ptr_physics.index();
		if(physicsAttributeIndex < physicsObjects_->size() && physicsAttributeIndex > -1)
		{
			PhysicsObject* physicsObject = physicsObjects_->at(physicsAttributeIndex);
			if(physicsObject != NULL)
			{
				//Cast void pointer sent in Event_ModifyPhysicsObject to its expected data type and modify physics object accordingly
				switch(modifyPhysicsObject->modifyWhatDataInPhysicsObjectData)
				{
				case XKILL_Enums::ModifyPhysicsObjectData::GRAVITY:
					{
						Float3* gravity = static_cast<Float3*>(modifyPhysicsObject->data);
						
						physicsObject->setGravity(btVector3(gravity->x, gravity->y, gravity->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::VELOCITY:
					{
						Float3* velocity = static_cast<Float3*>(modifyPhysicsObject->data);
						
						physicsObject->setLinearVelocity(btVector3(velocity->x, velocity->y, velocity->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::VELOCITYPERCENTAGE:
					{
						Float3* velocityPercentage = static_cast<Float3*>(modifyPhysicsObject->data);
						
						btVector3 currentLinearVelocity = physicsObject->getLinearVelocity();
						physicsObject->setLinearVelocity(btVector3(currentLinearVelocity.x()*velocityPercentage->x, currentLinearVelocity.y()*velocityPercentage->y, currentLinearVelocity.z()*velocityPercentage->z));
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::FLAG_STATIC:
					{
						bool* staticPhysicsObject = static_cast<bool*>(modifyPhysicsObject->data);
						
						if(*staticPhysicsObject == true)
						{
							physicsObject->setCollisionFlags(physicsObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
						}
						else if(*staticPhysicsObject == false)
						{
							physicsObject->setCollisionFlags(physicsObject->getCollisionFlags() & ~ btCollisionObject::CF_STATIC_OBJECT);
						}
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::COLLISIONFILTERMASK:
					{
						/*In order to modify "collisionFilterMask", a physics objects needs to be removed from the Bullet Physics dynamics world and then readded using "addRigidBody", where "collisionFilterMask" is passed as argument.
						Write physics object data to physics attribute, modify "collisionFilterMask", and set the "reloadDataIntoBulletPhysics" flag, and this class will handle the removal and addition of the physics object.*/
						
						short* collisionFilterMaskFromEvent = static_cast<short*>(modifyPhysicsObject->data);

						AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.at(physicsAttributeIndex);
						physicsObject->writeNonSynchronizedPhysicsObjectDataToPhysicsAttribute();
						ptr_physics->collisionFilterMask = *collisionFilterMaskFromEvent;
						ptr_physics->reloadDataIntoBulletPhysics = true;
					}
					break;
				case XKILL_Enums::ModifyPhysicsObjectData::GIVE_IMPULSE:
					{
						Float3* impulseVector = static_cast<Float3*>(modifyPhysicsObject->data);

						btVector3 impulse = convert(*impulseVector);
						physicsObject->applyCentralImpulse(impulse);
						break;
					}
				case XKILL_Enums::ModifyPhysicsObjectData::IF_TRUE_RECALCULATE_LOCAL_INERTIA_ELSE_SET_TO_ZERO:
					{
						bool* recalculateLocalInertia = static_cast<bool*>(modifyPhysicsObject->data);

						btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);
						btScalar mass = itrPhysics.at(physicsAttributeIndex)->mass;
						if(*recalculateLocalInertia == true)
						{
							btCollisionShape* collisionShape = physicsObject->getCollisionShape();
							collisionShape->calculateLocalInertia(mass, localInertia);

							physicsObject->setMassProps(mass, localInertia);
							physicsObject->updateInertiaTensor();
						}
						else
						{
							physicsObject->setMassProps(mass, localInertia);
						}

						break;
					}
				}
			}
			else
			{
				ERROR_MESSAGEBOX("Invalid physics attribute id when handling event of type EVENT_MODIFY_PHYSICS_OBJECT, error 1");
			}
		}
		else
		{
			ERROR_MESSAGEBOX("Invalid physics attribute id when handling event of type EVENT_MODIFY_PHYSICS_OBJECT, error 2");
		}
		break;
	}
	case EVENT_CLOSEST_HIT_RAY_CAST:
		{
			Event_ClosestHitRayCast* event_ClosestRayCast = static_cast<Event_ClosestHitRayCast*>(e);
			handleEvent_ClosestRayCast(event_ClosestRayCast);
		break;
		}
	case EVENT_ALL_HITS_RAY_CAST:
		{
			Event_AllHitsRayCast* event_AllHitsRayCast = static_cast<Event_AllHitsRayCast*>(e);
			handleEvent_AllHitsRayCast(event_AllHitsRayCast);
		break;
		}
	case EVENT_LOAD_LEVEL_BULLET:
		CollisionShapes::Instance()->loadCollisionShapes();
		break;
	case EVENT_UNLOAD_LEVEL:
		CollisionShapes::Instance()->unloadCollisionShapes();
		break;
	case EVENT_NULL_PROCESS_STOPPED_EXECUTING:
		{
			//Reset apart-fallen world
			while(itrPhysics.hasNext())
			{
				AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
				if(ptr_physics->collisionFilterGroup == XKILL_Enums::PhysicsAttributeType::PROP)
				{
					if(physicsObjects_->at(ptr_physics.index()) != nullptr)
					{
						PropPhysicsObject* propPhysicsObject = static_cast<PropPhysicsObject*>(physicsObjects_->at(ptr_physics.index()));
						
						ptr_physics->collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::WORLD;
						ptr_physics->collisionFilterMask = XKILL_Enums::PhysicsAttributeType::PLAYER | XKILL_Enums::PhysicsAttributeType::PROJECTILE |
							XKILL_Enums::PhysicsAttributeType::FRUSTUM | XKILL_Enums::PhysicsAttributeType::PICKUPABLE |
							XKILL_Enums::PhysicsAttributeType::RAY | XKILL_Enums::PhysicsAttributeType::PROP;

						ptr_physics->ptr_spatial->ptr_position->position = Float3(propPhysicsObject->worldOrigin_.x(),propPhysicsObject->worldOrigin_.y(),propPhysicsObject->worldOrigin_.z());
					
						propPhysicsObject->setCollisionFlags(propPhysicsObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);//check, might not be needed

						ptr_physics->gravity = Float3(0.0f, 0.0f, 0.0f);
						ptr_physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
						ptr_physics->mass = 0;
						ptr_physics->collisionResponse = true;

						SEND_EVENT(&Event_ReloadPhysicsAttributeDataIntoBulletPhysics(ptr_physics.index()));
					}
				}
			}
		break;
		}
	case EVENT_RELOAD_PHYSICS_ATTRIBUTE_DATA_INTO_BULLET_PHYSICS:
		{
			Event_ReloadPhysicsAttributeDataIntoBulletPhysics* event_ReloadPhysicsAttributeDataIntoBulletPhysics = static_cast<Event_ReloadPhysicsAttributeDataIntoBulletPhysics*>(e);
			int physicsAttributeId = event_ReloadPhysicsAttributeDataIntoBulletPhysics->physicsAttributeId;
			AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.at(physicsAttributeId);

			ptr_physics->reloadDataIntoBulletPhysics = true;
			synchronizeWithAttributes(ptr_physics, physicsAttributeId);

			break;
		}
	}
}

void PhysicsComponent::handleEvent_ClosestRayCast(Event_ClosestHitRayCast* event_ClosestRayCast)
{
	btVector3 from = convert(event_ClosestRayCast->from);
	btVector3 to = convert(event_ClosestRayCast->to);

	btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal; //check, what is this? (2013-02-20 11.24)
	closestResults.m_collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::RAY;
	closestResults.m_collisionFilterMask = event_ClosestRayCast->collisionFilterMask;

	dynamicsWorld_->rayTest(from, to, closestResults); //Bullet Physics ray cast

	//--------------------------------------------------------------------------------------
	// Special case event handling: the result of the event is stored in the event.
	//--------------------------------------------------------------------------------------
	if(closestResults.hasHit())
	{
		const PhysicsObject* hitObject = static_cast<const PhysicsObject*>(closestResults.m_collisionObject);
		event_ClosestRayCast->EntityIdOfOwnerToClosestPhysicsObjectHitByRay = itrPhysics.ownerIdAt(hitObject->getAttributeIndex());
			
		const btVector3 closestHitPoint = from.lerp(to,closestResults.m_closestHitFraction);
		event_ClosestRayCast->ClosestHitPoint = convert(&closestHitPoint);
	}
	else //Ray did not hit anything
	{
		event_ClosestRayCast->EntityIdOfOwnerToClosestPhysicsObjectHitByRay = 0; //0 denotes that no physics object was hit by the ray
		event_ClosestRayCast->ClosestHitPoint = event_ClosestRayCast->to; //There was no closest hit point, i.e. the closest hit point was the destination point of the ray
	}
}

struct sort_mapHitPointToEntityId
{
    bool operator()(std::pair<Float3, int> &left, std::pair<Float3, int> &right)
	{
        return left.first < right.first;
    }
};
void PhysicsComponent::handleEvent_AllHitsRayCast(Event_AllHitsRayCast* event_AllHitsRayCast)
{
	btVector3 from = convert(event_AllHitsRayCast->from);
	btVector3 to = convert(event_AllHitsRayCast->to);

	btCollisionWorld::AllHitsRayResultCallback allResults(from,to);
	allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal; //check, what is this? (2013-02-28 11.53)
	allResults.m_collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::RAY;
	allResults.m_collisionFilterMask = event_AllHitsRayCast->collisionFilterMask;

	dynamicsWorld_->rayTest(from,to,allResults); //Bullet Physics ray cast

	//--------------------------------------------------------------------------------------
	// Special case event handling: the result of the event is stored in the event.
	//--------------------------------------------------------------------------------------
	int nrOfHits = allResults.m_hitFractions.size();
	for(int i=0;i<nrOfHits;i++)
	{
		const PhysicsObject* hitObject = static_cast<const PhysicsObject*>(allResults.m_collisionObjects.at(i));
		int entityId = itrPhysics.ownerIdAt(hitObject->getAttributeIndex());

		const btVector3 currentHitPoint = from.lerp(to,allResults.m_hitFractions.at(i));
		Float3 hitPoint = convert(&currentHitPoint);

		event_AllHitsRayCast->mapHitPointToEntityId.push_back(std::pair<Float3, int>(hitPoint, entityId));
	}
	//--------------------------------------------------------------------------------------
	// Sort mapHitPointToEntityId so that the closer hitpoint has lower vector indices
	//--------------------------------------------------------------------------------------
	std::sort(event_AllHitsRayCast->mapHitPointToEntityId.begin(), event_AllHitsRayCast->mapHitPointToEntityId.end(), sort_mapHitPointToEntityId());
}

void PhysicsComponent::synchronizeWithAttributes(AttributePtr<Attribute_Physics> ptr_physics, int physicsAttributeIndex)
{
	//Also refer to PhysicsComponent::onEvent, handling of EVENT_ATTRIBUTE_UPDATED

	//Checks if new physiscs attributes were created since last call to this function
	if(physicsAttributeIndex >= physicsObjects_->size())
	{
		physicsObjects_->push_back(nullptr);
	}
	//Synchronize physics attributes with internal PhysicsObjects
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
		case XKILL_Enums::PhysicsAttributeType::WORLD:
			physicsObjects_->at(physicsAttributeIndex) = new WorldPhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::PLAYER:
			physicsObjects_->at(physicsAttributeIndex) = new PlayerPhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::PROJECTILE:
			physicsObjects_->at(physicsAttributeIndex) = new ProjectilePhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::EXPLOSIONSPHERE:
			physicsObjects_->at(physicsAttributeIndex) = new ExplosionSpherePhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::PICKUPABLE:
			physicsObjects_->at(physicsAttributeIndex) = new PickupablePhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::PROP:
			physicsObjects_->at(physicsAttributeIndex) = new PropPhysicsObject();
			break;
		case XKILL_Enums::PhysicsAttributeType::EVERYTHING:
			ERROR_MESSAGEBOX("Error: Attribute_Physics should not have EVERYTHING as collisionFilterGroup");
			break;
		}

		if(physicsObjects_ != nullptr)
		{
			if(physicsObjects_->at(physicsAttributeIndex)->init(physicsAttributeIndex, ptr_physics->collisionFilterGroup) == true)
			{
				dynamicsWorld_->addRigidBody(physicsObjects_->at(physicsAttributeIndex), ptr_physics->collisionFilterGroup, ptr_physics->collisionFilterMask);

				//Per object gravity must be set after "btDiscreteDynamicsWorld::addRigidBody"
				if(!physicsObjects_->at(physicsAttributeIndex)->isStaticOrKinematicObject())
				{
					physicsObjects_->at(physicsAttributeIndex)->setGravity(btVector3(ptr_physics->gravity.x, ptr_physics->gravity.y, ptr_physics->gravity.z));
				}

				ptr_physics->reloadDataIntoBulletPhysics = false;
			}
			else
			{
				ERROR_MESSAGEBOX("-->Error initializing PhysicsObject. PhysicsComponent::synchronizeWithAttributes failed");
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

				//--------------------------------------------------------------------------------------
				// Global frustum culling
				//--------------------------------------------------------------------------------------
				//check physicsobjecttype;
				//if(objectA->getCollisionFilterGroup() == XKILL_Enums::PhysicsAttributeType::FRUSTUM ||
				//	objectB->getCollisionFilterGroup() == XKILL_Enums::PhysicsAttributeType::FRUSTUM)
				//{
				//	if(objectA->getCollisionFilterGroup() == XKILL_Enums::PhysicsAttributeType::FRUSTUM &&
				//	   objectB->getCollisionFilterGroup() != XKILL_Enums::PhysicsAttributeType::FRUSTUM)
				//	{
				//		doCulling(objectA->getAttributeIndex(),objectB->getAttributeIndex());
				//	}
				//	else if(objectA->getCollisionFilterGroup() != XKILL_Enums::PhysicsAttributeType::FRUSTUM &&
				//	   objectB->getCollisionFilterGroup() == XKILL_Enums::PhysicsAttributeType::FRUSTUM)
				//	{
				//		doCulling(objectB->getAttributeIndex(),objectA->getAttributeIndex());
				//	}
				//}
				//else
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
	if(itrPhysics.at(objectAttributeIndex)->ptr_render.isValid())
	{
		itrPhysics.at(objectAttributeIndex)->ptr_render->cull = true;
	}
}

void PhysicsComponent::updateCulling()
{
	while(itrPhysics.hasNext())
	{
		AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
		if(ptr_physics->ptr_render.isValid())
		{
			ptr_physics->ptr_render->cull = false;
		}
	}
	CollisionShapes::Instance()->updateFrustumShape();

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
			frustumPhysicsObjects_->at(index)->frustumInit(index,XKILL_Enums::PhysicsAttributeType::FRUSTUM);
			int a = dynamicsWorld_->getNumCollisionObjects();
			dynamicsWorld_->addRigidBody(frustumPhysicsObjects_->at(index),XKILL_Enums::PhysicsAttributeType::FRUSTUM,XKILL_Enums::PhysicsAttributeType::EVERYTHING);
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