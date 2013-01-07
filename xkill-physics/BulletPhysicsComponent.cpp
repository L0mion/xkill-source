#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch\btGhostObject.h>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>

#include "CollisionShapeManager.h"
#include "physicsObject.h"
#include "physicsUtilities.h"

#include <xkill-utilities/EventManager.h>

#include <xkill-utilities/DebugShape.h>

#include <iostream>

#define SAFE_DELETE(obj)	if(obj != nullptr) { delete obj;		obj = nullptr; }


BulletPhysicsComponent::BulletPhysicsComponent()
{
	SUBSCRIBE_TO_EVENT(this,EVENT_DO_CULLING);

	inputAttributes_ = nullptr;
	physicsAttributes_ = nullptr;
	boundingAttributes_ = nullptr;
	
	broadphase_ = nullptr;
	collisionConfiguration_ = nullptr;
	dispatcher_ = nullptr;
	solver_ = nullptr;
	dynamicsWorld_ = nullptr;
	physicsObjects_ = nullptr;
	frustrumObjects_ = nullptr;
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
	SAFE_DELETE(physicsObjects_);
	
	if(frustrumObjects_ != nullptr)
	{
		while(frustrumObjects_->size() > 0)
		{
			frustrumObjects_->at(frustrumObjects_->size()-1)->removeFromWorld(dynamicsWorld_);
			delete frustrumObjects_->at(frustrumObjects_->size()-1)->getCollisionShape();
			SAFE_DELETE(frustrumObjects_->at(frustrumObjects_->size()-1));
			frustrumObjects_->pop_back();
		}
	}
	SAFE_DELETE(frustrumObjects_);

	dynamicsWorld_->removeRigidBody(floor_);
	delete floor_->getCollisionShape();

	SAFE_DELETE(floor_);

    SAFE_DELETE(solver_);
    SAFE_DELETE(collisionConfiguration_);
    SAFE_DELETE(dispatcher_);
    SAFE_DELETE(broadphase_);
	SAFE_DELETE(dynamicsWorld_);
	//SAFE_DELETE(ghostPairCallback_);
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
	inputAttributes_		= GET_ATTRIBUTES(input);
	physicsAttributes_		= GET_ATTRIBUTES(physics);
	boundingAttributes_		= GET_ATTRIBUTES(bounding);
	renderAttributes_		= GET_ATTRIBUTES(render);
	cameraAttributes_		= GET_ATTRIBUTES(camera);
	debugShapeAttributes_	= GET_ATTRIBUTES(debugShape);
	physicsOwners_			= GET_ATTRIBUTE_OWNERS(physics);
	
	physicsObjects_ = new btAlignedObjectArray<PhysicsObject*>();
	frustrumObjects_ = new btAlignedObjectArray<PhysicsObject*>();
	broadphase_ = new btDbvtBroadphase();
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
	solver_ = new btSequentialImpulseConstraintSolver;
	dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
	collisionShapeManager_ = new CollisionShapeManager();

	dynamicsWorld_->setGravity(btVector3(0,0,0));
	dynamicsWorld_->setInternalTickCallback(wrapTickCallback,static_cast<void*>(this));


	floor_ = new PhysicsObject(new btStaticPlaneShape(btVector3(0,1,0),0.0f),1337,tStatic);
	dynamicsWorld_->addRigidBody(floor_);

	//ghostObjects_ = new btAlignedObjectArray<btGhostObject*>;
	//ghostPairCallback_ = new btGhostPairCallback;
	//dynamicsWorld_->getPairCache()->setInternalGhostPairCallback(ghostPairCallback_);
	
	return true;
}


void BulletPhysicsComponent::onUpdate(float delta)
{
	//Checks if new physiscs attributes were created since last call to this function
	for(unsigned int i = physicsObjects_->size(); i < physicsAttributes_->size(); i++)
	{
		physicsObjects_->push_back(new PhysicsObject(collisionShapeManager_,i,PO_Types::tDEFAULT));
	}

	for(unsigned int i = 0; i < inputAttributes_->size(); i++)
	{
		if(i < static_cast<unsigned int>(physicsObjects_->size()))
			physicsObjects_->at(inputAttributes_->at(i).ptr_physics.index)->input(&inputAttributes_->at(i),delta);
	}
	
	//for(unsigned int i = 0; i < debugShapeAttributes_->size(); i++)
	//{
	//	DebugShapeAttribute *debugShapeAttribute = &debugShapeAttributes_->at(i);
	//	if(!debugShapeAttribute->render)
	//	{
	//		btTransform c;
	//		btVector3 a,b,d;
	//		float e;
	//		collisionShapeManager_->getCollisionShape(debugShapeAttribute->meshID)->getAabb(c,a,b);
	//		a = (1.0f/100.0f)*a;
	//		b = (1.0f/100.0f)*b;
	//		
	//		collisionShapeManager_->getCollisionShape(debugShapeAttribute->meshID)->getBoundingSphere(d,e);
	//		e = (1.0f/100.0f)*e;
	//		//debugShapeAttribute->shape = new DebugShapeBB(Float3(a.x(),a.y(),a.z()),Float3(b.x(),b.y(),b.z()));

	//		debugShapeAttribute->render = true;
	//		debugShapeAttribute->shape = new DebugShapeSphere(e);
	//	}
	//}

	//Synchronize the internal represenation of physics objects with the physics attributes
	for(unsigned int i = 0; i < static_cast<unsigned int>(physicsObjects_->size()); i++)
	{
		PhysicsObject* physicsObject = physicsObjects_->at(i);
		Attribute_Physics* physicsAttribute = &physicsAttributes_->at(i);
		// if the objects owner is not 0 it should simulate
		if(physicsOwners_->at(i)!=0)
		{
			// if the object is not in the world, add it
			if(!physicsObject->isInWorld())
			{
				if(physicsAttribute->isExplosionSphere)
				{
					float scale = 100.0f;
					btCollisionShape* collisionSphere = new btSphereShape(physicsAttribute->explosionSphereRadius*scale);
					collisionSphere->setLocalScaling(btVector3(scale,scale,scale));
					collisionShapeManager_->addCollisionShape(collisionSphere);
					physicsObject->setCollisionShape(collisionSphere);
				}

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
		Attribute_Physics* physicsAttribute = &physicsAttributes_->at(i);
		if(physicsOwners_->at(i)!=0 && physicsObject->isInWorld())
		{
			physicsObject->postStep(physicsAttribute);
		}
	}

	FLUSH_QUEUED_EVENTS(EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
}

void BulletPhysicsComponent::onEvent(Event* e)
{
	//for(unsigned int i = 0; i < renderAttributes_->size(); i++)
	//{
	//	renderAttributes_->at(i).culling.clear();
	//}
	//for(unsigned int i = frustrumObjects_->size(); i < cameraAttributes_->size(); i++)
	//{
	//	//create new frustrum;
	//	CameraAttribute* cameraAttribute = &cameraAttributes_->at(i);
	//	btConvexHullShape* frustumShape = new btConvexHullShape;
	//	float far = cameraAttribute->zFar;
	//	float near = cameraAttribute->zNear;
	//	float aspect = cameraAttribute->aspect;
	//	float fovy = cameraAttribute->fov;
	//	float fovx = 2*atan(aspect*tan(fovy/2));

	//	frustumShape->addPoint(WorldScaling*btVector3(near*tan(fovx/2),near*tan(fovy/2),near));    //y = far/ tan(fov/2)
	//	frustumShape->addPoint(WorldScaling*btVector3(-near*tan(fovx/2),near*tan(fovy/2),near));   //fovx = 2atan(aspect*tan(fovy/2))
	//	frustumShape->addPoint(WorldScaling*btVector3(near*tan(fovx/2),-near*tan(fovy/2),near));
	//	frustumShape->addPoint(WorldScaling*btVector3(-near*tan(fovx/2),-near*tan(fovy/2),near));

	//	frustumShape->addPoint(WorldScaling*btVector3(far*tan(fovx/2),far*tan(fovy/2),far));
	//	frustumShape->addPoint(WorldScaling*btVector3(-far*tan(fovx/2),far*tan(fovy/2),far));
	//	frustumShape->addPoint(WorldScaling*btVector3(far*tan(fovx/2),-far*tan(fovy/2),far));
	//	frustumShape->addPoint(WorldScaling*btVector3(-far*tan(fovx/2),-far*tan(fovy/2),far));

	//	PhysicsObject *frustrum = new PhysicsObject(frustumShape,i,PO_Types::tFrustrum);
	//	delete frustumShape;

	//	frustrum->setCollisionShape(new btSphereShape(100));
	//	
	//	btTransform test;
	//	test.setIdentity();
	//	frustrum->setWorldTransform(test);
	//	frustrum->setLinearVelocity(btVector3(0,0,0));
	//	frustrum->setAngularVelocity(btVector3(0,0,0));
	//	frustrum->setMassProps(1,btVector3(0,0,0));
	//	frustrum->setCollisionFlags(frustrum->getCollisionFlags() | frustrum->CF_NO_CONTACT_RESPONSE);
	//	frustrumObjects_->push_back(frustrum);
	//}
	//for(unsigned int i = 0; i < frustrumObjects_->size(); i++)
	//{
	//	PhysicsObject* frustrum = frustrumObjects_->at(i);
	//	CameraAttribute* camera = &cameraAttributes_->at(i);
	//	SpatialAttribute* spatial = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,camera);
	//	PositionAttribute* position = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,spatial);
	//	Float3 look (camera->mat_view._13, camera->mat_view._23, camera->mat_view._33);
	//	Float3 eye (camera->mat_view._13, camera->mat_view._23, camera->mat_view._33);
	//	
	//	
	//	/*float yaw = atan(-look.x/look.y);
	//	float pitch = atan(sqrt(look.x*look.x+look.y*look.y)/look.z);*/
	//	float yaw,pitch,distance;
	//	distance = sqrt(look.x*look.x + look.z*look.z);
	//	pitch = atan2(look.y,distance);
	//	yaw = atan2(look.x, look.z);
	//	
	//	
	//	//frustrum->set
	//	frustrum->getWorldTransform().setRotation(btQuaternion(yaw,
	//														 pitch,
	//														 0));
	//	frustrum->getWorldTransform().setOrigin(btVector3(position->position.x, //+ look.x*0.1f,
	//													  position->position.y, //+ look.y*0.1f,
	//													  position->position.z)); //+ look.z*0.1f));
	//	//frustrum->setCollisionFlags( frustrum->getCollisionFlags() | frustrum->CF_NO_CONTACT_RESPONSE);
	//	dynamicsWorld_->addRigidBody(frustrum);
	//}
	//dynamicsWorld_->performDiscreteCollisionDetection();
	//tickCallback(0);
	//for(unsigned int i = 0; i < frustrumObjects_->size(); i++)
	//{
	//	dynamicsWorld_->removeRigidBody(frustrumObjects_->at(i));
	//}
	//FLUSH_QUEUED_EVENTS(EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
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
			if(persistentManifold->getContactPoint(j).getDistance()<0.0f)
			{
				const PhysicsObject* objectA = static_cast<const PhysicsObject*>(persistentManifold->getBody0());
				const PhysicsObject* objectB = static_cast<const PhysicsObject*>(persistentManifold->getBody1());
				if( objectA->getType() != PO_Types::tStatic && objectB->getType() != PO_Types::tStatic)
				{
					if( objectA->getType() == PO_Types::tFrustrum || objectB->getType() == PO_Types::tFrustrum)
					{
						if(!(objectA->getType() == PO_Types::tFrustrum && objectB->getType() == PO_Types::tFrustrum))
						{
							if(objectA->getType() == PO_Types::tFrustrum)
							{
								Attribute_Physics* physics = &physicsAttributes_->at(objectB->getIndex());
								Attribute_Render* render; render = ATTRIBUTE_CAST(Attribute_Render, ptr_render, physics);
								render->culling.setBool(objectA->getIndex(),true);
							}
							else
							{
								Attribute_Physics* physics = &physicsAttributes_->at(objectA->getIndex());
								Attribute_Render* render; render = ATTRIBUTE_CAST(Attribute_Render, ptr_render, physics);
								render->culling.setBool(objectB->getIndex(),true);
							}
						}
					
					}
					else
					{
						unsigned int ownerA = physicsOwners_->at(objectA->getIndex());
						unsigned int ownerB = physicsOwners_->at(objectB->getIndex());
					
						//Two PhysicsObjects colliding
						if(ownerA != 0 || ownerB != 0) // ignore contacts where one owner is 0
						{
							QUEUE_EVENT(new Event_PhysicsAttributesColliding(objectA->getIndex(), objectB->getIndex()));
							QUEUE_EVENT(new Event_PhysicsAttributesColliding(objectB->getIndex(), objectA->getIndex()));
						}
					}
				}
			}
		}
	}
}