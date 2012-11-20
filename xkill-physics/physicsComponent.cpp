#include "physicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include "attributes.h"
#include "physicsObject.h"

PhysicsComponent::PhysicsComponent()
{
	broadphase = nullptr;
	collisionConfiguration = nullptr;
	dispatcher = nullptr;
	solver = nullptr;
	dynamicsWorld = nullptr;
	physicsAttributes = nullptr;
	physicsObjects = nullptr;
	numPhysicsAttributes = 0;
}

PhysicsComponent::~PhysicsComponent()
{
	//delete local objects etc.
 
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

bool PhysicsComponent::Init()
{
        broadphase = new btDbvtBroadphase();
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        solver = new btSequentialImpulseConstraintSolver;
        
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0,-10,0));
		
		//ground = new PhysicsObject;
		//ground->Init(new btStaticPlaneShape(btVector3(0,1,0),1),
		//			 new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0))),
		//			 0,
		//			 btVector3(0,0,0),
		//			 dynamicsWorld);

		//fall = new PhysicsObject;
		//fall->Init(new btSphereShape(1),
		//		   new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0))),
		//		   1,
		//		   btVector3(0,0,0),
		//		   dynamicsWorld);

        return true;
}

void PhysicsComponent::onUpdate(float delta)
{
	for(unsigned int i = 0; i < numPhysicsAttributes; i++)
	{
		if((*physicsAttributes)[i].alive)
		{
			if((*physicsAttributes)[i].added)
			{
				(*physicsObjects)[i]->preStep(&(*physicsAttributes)[i]);
			}
			else
			{
				if((*physicsAttributes).size()>i)
				{
					(*physicsAttributes)[i].added = true;
					//init??
				}
				else
				{
					physicsObjects->push_back(new PhysicsObject());
				}
			}
		}
		else if((*physicsAttributes)[i].added)
		{
			(*physicsObjects)[i]->Clean(dynamicsWorld);
			(*physicsAttributes)[i].added = false;
		}
	}

	dynamicsWorld->stepSimulation(delta,10);

	for(unsigned int i = 0; i < numPhysicsAttributes; i++)
	{
		if((*physicsAttributes)[i].alive && (*physicsAttributes)[i].added)
		{
			(*physicsObjects)[i]->postStep(&(*physicsAttributes)[i]);
		}
	}
}

void PhysicsComponent::onEvent(/*Event* e*/)
{

}