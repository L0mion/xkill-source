#include "physicsObject.h"

#include <xkill-utilities/AttributeType.h>
#include <vector>
#include <iostream>

#include "CollisionShapeManager.h"

PhysicsObject::PhysicsObject(CollisionShapeManager* collisionShapeManager, unsigned int index) : btRigidBody(1,
																											 new btDefaultMotionState(),
																											 collisionShapeManager->getCollisionShape(0),
																											 btVector3(0,0,0))
{
	index_ = index;
	gravity_.setZero();
	forces_.setZero();
	movement_.setZero();
	yaw_ = 0.0f;
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}

void PhysicsObject::addToWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->addRigidBody(this);
}

void PhysicsObject::removeFromWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(this);
}

void PhysicsObject::preStep(CollisionShapeManager* collisionShapeManager,PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,
														spatialAttribute,
														physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,
														  positionAttribute,
														  spatialAttribute);
	setMassProps(physicsAttribute->mass,btVector3(0,0,0));
	movement_.setY(physicsAttribute->linearVelocity.y);
	m_worldTransform.setOrigin(btVector3(positionAttribute->position.x,
										 positionAttribute->position.y,
										 positionAttribute->position.z));
	m_worldTransform.setRotation(btQuaternion(yaw_,0,0));
	setLinearVelocity(movement_);
	setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
					   physicsAttribute->angularVelocity.y,
					   physicsAttribute->angularVelocity.z));
	m_collisionShape = collisionShapeManager->getCollisionShape(physicsAttribute->collisionShapeIndex);
	setGravity(gravity_+forces_);
	activate(true);
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,
														spatialAttribute,
														physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,
														  positionAttribute,
														  spatialAttribute);
	positionAttribute->position.copy(m_worldTransform.getOrigin().m_floats);
	spatialAttribute->rotation.copy(m_worldTransform.getRotation().get128().m128_f32);
	physicsAttribute->linearVelocity.copy(getLinearVelocity().m_floats);
	physicsAttribute->angularVelocity.copy(getAngularVelocity().m_floats);
	forces_.setZero();
}

void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	yaw_ += inputAttribute->rotation.x;
	movement_ = btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	movement_ = 20*movement_.rotate(btVector3(0,1,0),yaw_);
	inputAttribute->position.x = inputAttribute->position.y = 0;
}

bool PhysicsObject::contactTest(btDiscreteDynamicsWorld* dynamicsWorld, PhysicsObject& otherPhysicsObject)
{
	//check convexSweepTest
	
	CollisionResult collisionResult;
	dynamicsWorld->contactPairTest(this,&otherPhysicsObject, collisionResult);

	return collisionResult.collision;
}

unsigned int PhysicsObject::getIndex() const
{
	return index_;
}