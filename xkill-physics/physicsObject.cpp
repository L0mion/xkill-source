#include "physicsObject.h"

#include <xkill-utilities/AttributeType.h>
#include <vector>
#include <iostream>

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

void PhysicsObject::Init(PhysicsAttribute* physicsAttribute, btDiscreteDynamicsWorld* dynamicsWorld)
{
	btTransform transform;
	transform.setOrigin(btVector3(0,0,-90));
	transform.setRotation(btQuaternion(0,0,0,1));
	rigidBody_ = new btRigidBody(physicsAttribute->mass,
		new btDefaultMotionState(transform),
		new btSphereShape(1), //CollisionShape
		btVector3(0,0,0));
	//rigidBody_->updateInertiaTensor();

	dynamicsWorld->addRigidBody(rigidBody_);

	forces_ = btVector3(0,0,0);
	movement_ = btVector3(0,0,0);
	yaw_ = 0;
	preStep(physicsAttribute);
}

void PhysicsObject::Clean(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(rigidBody_);
}



void PhysicsObject::preStep(PhysicsAttribute* physicsAttribute)
{
	btVector3 position;
	btQuaternion rotation;
	btVector3 linearVelocity;
	btVector3 angularVelocity;
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,spatialAttribute);
	
	//Copy from attributes to Bullet Physics internal representation
	memcpy(position,&positionAttribute->position,3*sizeof(float));
	memcpy(rotation,&spatialAttribute->rotation,4*sizeof(float));
	memcpy(linearVelocity,&physicsAttribute->linearVelocity,3*sizeof(float));
	memcpy(angularVelocity,&physicsAttribute->angularVelocity,3*sizeof(float));

	//memcpy(position,positionAttribute->position,3*sizeof(float));
	//memcpy(rotation,spatialAttribute->rotation,4*sizeof(float));
	//memcpy(linearVelocity,physicsAttribute->linearVelocity,3*sizeof(float));
	//memcpy(angularVelocity,physicsAttribute->angularVelocity,3*sizeof(float));

	position = btVector3(positionAttribute->position.x,
						 positionAttribute->position.y,
						 positionAttribute->position.z);
	rotation = btQuaternion(yaw_,0,0);
	linearVelocity = btVector3(physicsAttribute->linearVelocity.x,
							   physicsAttribute->linearVelocity.y,
							   physicsAttribute->linearVelocity.z);
	angularVelocity = btVector3(physicsAttribute->angularVelocity.x,
							    physicsAttribute->angularVelocity.y,
							    physicsAttribute->angularVelocity.z);

	btVector3 gravity(0,-10,0);
	movement_.setY(linearVelocity.y());
	linearVelocity = movement_;
	forces_ = linearVelocity;

	rigidBody_->setWorldTransform(btTransform(rotation,position));
	rigidBody_->setLinearVelocity(linearVelocity);
	rigidBody_->setAngularVelocity(angularVelocity);
	rigidBody_->setMassProps(physicsAttribute->mass,btVector3(0,0,0));
	rigidBody_->setGravity(gravity+forces_);
	rigidBody_->activate(true);
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,spatialAttribute);

	//memcpy(positionAttribute->position,rigidBody_->getWorldTransform().getOrigin(),3*sizeof(float));
	//memcpy(spatialAttribute->rotation,rigidBody_->getWorldTransform().getRotation(),4*sizeof(float));
	//memcpy(physicsAttribute->linearVelocity,rigidBody_->getLinearVelocity(),3*sizeof(float));
	//memcpy(physicsAttribute->angularVelocity,rigidBody_->getAngularVelocity(),3*sizeof(float));

	positionAttribute->position.x = rigidBody_->getCenterOfMassPosition().x();
	positionAttribute->position.y = rigidBody_->getCenterOfMassPosition().y();
	positionAttribute->position.z = rigidBody_->getCenterOfMassPosition().z();
	spatialAttribute->rotation.x = rigidBody_->getWorldTransform().getRotation().x();
	spatialAttribute->rotation.y = rigidBody_->getWorldTransform().getRotation().y();
	spatialAttribute->rotation.z = rigidBody_->getWorldTransform().getRotation().z();
	spatialAttribute->rotation.w = rigidBody_->getWorldTransform().getRotation().w();
	physicsAttribute->linearVelocity.x = rigidBody_->getLinearVelocity().x();
	physicsAttribute->linearVelocity.y = rigidBody_->getLinearVelocity().y();
	physicsAttribute->linearVelocity.z = rigidBody_->getLinearVelocity().z();
	physicsAttribute->angularVelocity.x = rigidBody_->getAngularVelocity().x();
	physicsAttribute->angularVelocity.y = rigidBody_->getAngularVelocity().y();
	physicsAttribute->angularVelocity.z = rigidBody_->getAngularVelocity().z();

	forces_ = btVector3(0,0,0);
}

void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	yaw_ += inputAttribute->rotation.x;
	movement_ = btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	movement_ = movement_.rotate(btVector3(0,1,0),yaw_);
	//forces_ =movement_;
	inputAttribute->position.x = inputAttribute->position.y = 0;
}

bool PhysicsObject::contactTest(btDiscreteDynamicsWorld* dynamicsWorld, const PhysicsObject& otherPhysicsObject)
{
	//check convexSweepTest
	
	CollisionResult collisionResult;
	dynamicsWorld->contactPairTest(rigidBody_,otherPhysicsObject.rigidBody_, collisionResult);

	return collisionResult.collision;
}
