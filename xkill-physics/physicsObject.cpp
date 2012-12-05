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
	transform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f)); //Important
	rigidBody_ = new btRigidBody(physicsAttribute->mass,
								 new btDefaultMotionState(transform),
								 new btSphereShape(50), //CollisionShape
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
	//memcpy(position,&positionAttribute->position,3*sizeof(float));
	position.setX(positionAttribute->position.x);
	position.setY(positionAttribute->position.y);
	position.setZ(positionAttribute->position.z);
	//memcpy(rotation,&spatialAttribute->rotation,4*sizeof(float));
	rotation.setX(spatialAttribute->rotation.x);
	rotation.setY(spatialAttribute->rotation.y);
	rotation.setZ(spatialAttribute->rotation.z);
	rotation.setW(spatialAttribute->rotation.w);
	//memcpy(linearVelocity,&physicsAttribute->linearVelocity,3*sizeof(float));
	linearVelocity.setX(physicsAttribute->linearVelocity.x);
	linearVelocity.setY(physicsAttribute->linearVelocity.y);
	linearVelocity.setZ(physicsAttribute->linearVelocity.z);
	//memcpy(angularVelocity,&physicsAttribute->angularVelocity,3*sizeof(float));
	angularVelocity.setX(physicsAttribute->angularVelocity.x);
	angularVelocity.setY(physicsAttribute->angularVelocity.y);
	angularVelocity.setZ(physicsAttribute->angularVelocity.z);

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

	btVector3 gravity(0,0,0);
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

	//Convert btTransform to Float3 and save in attributes
	//positionAttribute->position.copy(static_cast<float*>(rigidBody_->getWorldTransform().getOrigin()));
	positionAttribute->position.x = rigidBody_->getWorldTransform().getOrigin().getX();
	positionAttribute->position.y = rigidBody_->getWorldTransform().getOrigin().getY();
	positionAttribute->position.z = rigidBody_->getWorldTransform().getOrigin().getZ();

	//spatialAttribute->rotation.copy(static_cast<float*>(rigidBody_->getWorldTransform().getRotation()));
	spatialAttribute->rotation.x = rigidBody_->getWorldTransform().getRotation().getX();
	spatialAttribute->rotation.y = rigidBody_->getWorldTransform().getRotation().getY();
	spatialAttribute->rotation.z = rigidBody_->getWorldTransform().getRotation().getZ();
	spatialAttribute->rotation.w = rigidBody_->getWorldTransform().getRotation().getW();

	//Convert btVector3 to Float3 and save in attributes
	physicsAttribute->angularVelocity.copy(static_cast<float*>(btVector3(rigidBody_->getAngularVelocity())));
	physicsAttribute->linearVelocity.copy(static_cast<float*>(btVector3(rigidBody_->getLinearVelocity())));

	forces_ = btVector3(0,0,0);
}

void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	yaw_ += inputAttribute->rotation.x;
	movement_ = btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	movement_ = 100*movement_.rotate(btVector3(0,1,0),yaw_);
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