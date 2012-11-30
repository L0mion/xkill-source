#include "BulletPhysicsComponent.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeType.h>

#include "CollisionShapeManager.h"
#include "physicsObject.h"

BulletPhysicsComponent::BulletPhysicsComponent(std::vector<PhysicsAttribute>* physicsAttributes,
											   std::vector<BoundingAttribute>* boundingAttributes)
{
	physicsAttributes_ = physicsAttributes; 
	boundingAttributes_ = boundingAttributes;
	broadphase_ = nullptr;
	collisionConfiguration_ = nullptr;
	dispatcher_ = nullptr;
	solver_ = nullptr;
	dynamicsWorld_ = nullptr;
	physicsAttributes_ = nullptr;
	physicsObjects_ = nullptr;
	collisionShapeManager_ = nullptr;

}

BulletPhysicsComponent::~BulletPhysicsComponent()
{
	while(physicsObjects_->size() > 0)
	{
		delete physicsObjects_->at(physicsObjects_->size());
		physicsObjects_->pop_back();
	}
	delete physicsObjects_;

    delete dynamicsWorld_;
    delete solver_;
    delete collisionConfiguration_;
    delete dispatcher_;
    delete broadphase_;
	delete collisionShapeManager_;
}

bool BulletPhysicsComponent::init()
{
	physicsObjects_ = new btAlignedObjectArray<PhysicsObject*>();
	broadphase_ = new btDbvtBroadphase();
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
	solver_ = new btSequentialImpulseConstraintSolver;
	dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
	collisionShapeManager_ = new CollisionShapeManager;

	dynamicsWorld_->setGravity(btVector3(0,-10,0));

	//////

	//////
	struct vec3
	{
		float x_,y_,z_;
		vec3(){}
		vec3(float x, float y, float z)	{ x_=x;y_=y;z_=z; }
	};
	vec3* v = new vec3[9];
	
	v[0] = vec3(-1,-1,-1);	v[1] = vec3(-1,-1,1);
	v[2] = vec3(1,-1,-1);	v[3] = vec3(1,-1,1);

	v[4] = vec3(-1,1,-1);	v[5] = vec3(-1,1,1);
	v[6] = vec3(1,1,-1);	v[7] = vec3(1,1,1);

	v[8] = vec3(0.5f,0.5f,0.5f);
	//For each mesh create a bounding volume
	collisionShapeManager_->createConvexHull(&v[0].x_,3*9);
	delete [] v;
	
	//////

	//////	
	
	return true;
}

void BulletPhysicsComponent::onUpdate(float delta)
{
	for(unsigned int i = (*physicsObjects_).size(); i < physicsObjects_->size(); i++)
	{
		physicsObjects_->push_back(new PhysicsObject());
	}
	for(unsigned int i = 0; i < physicsObjects_->size(); i++)
	{
		if(physicsAttributes_->at(i).alive)
		{
			if(physicsAttributes_->at(i).added)
			{
				(*physicsObjects_)[i]->preStep(&physicsAttributes_->at(i));
			}
			else
			{
				(*physicsObjects_)[i]->Init(&physicsAttributes_->at(i),dynamicsWorld_);
				physicsAttributes_->at(i).added = true;
			}
		}
		else if(physicsAttributes_->at(i).added)
		{
			(*physicsObjects_)[i]->Clean(dynamicsWorld_);
			physicsAttributes_->at(i).added = false;
		}
	}

	dynamicsWorld_->stepSimulation(delta,10);

	for(unsigned int i = 0; i < physicsObjects_->size(); i++)
	{
		if(physicsAttributes_->at(i).alive && physicsAttributes_->at(i).added)
		{
			(*physicsObjects_)[i]->postStep(&physicsAttributes_->at(i));
		}
	}
}

void BulletPhysicsComponent::onEvent(Event* e)
{

}

void BulletPhysicsComponent::shootBox()
{
	//PhysicsObject class not yet completed (wrapper for btRigidBody)

		
		//this->localCreateRigidBody(mass, startTransform, m_shootBoxShape);
}