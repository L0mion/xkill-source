#include "physicsObject.h"

#include "attributes.h"
#include <btBulletDynamicsCommon.h>

PhysicsObject::PhysicsObject()
{
	rigidBody_ = nullptr;
}

PhysicsObject::~PhysicsObject()
{
	delete rigidBody_->getMotionState();
	delete rigidBody_->getCollisionShape();
	delete rigidBody_;
	rigidBody_ = nullptr;
}

void PhysicsObject::Clean(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(rigidBody_);
}

bool PhysicsObject::Init(btCollisionShape* collisionShape,
						 btDefaultMotionState* motionState,
						 btScalar mass,
						 btVector3 localInerta,
						 btDiscreteDynamicsWorld* dynamicsWorld)
{
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,collisionShape,localInerta);
	rigidBody_ = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody_);
}

void PhysicsObject::preStep(PhysicsAttribute* physicsAttribute)
{
	
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	
}