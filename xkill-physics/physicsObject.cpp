#include "physicsObject.h"

#include "attributes.h"

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

void PhysicsObject::Init(btCollisionShape* collisionShape,
						 btDefaultMotionState* motionState,
						 btScalar mass,
						 btVector3 &localInertia,
						 btDiscreteDynamicsWorld* dynamicsWorld)
{
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,collisionShape,localInertia);
	rigidBody_ = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody_);
}

void PhysicsObject::preStep(PhysicsAttribute* physicsAttribute)
{
	btTransform transform;
	rigidBody_->setWorldTransform(transform);
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	
}