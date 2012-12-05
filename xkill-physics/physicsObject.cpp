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
	btTransform a;
	a.setOrigin(btVector3(0,0,-90));
	a.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f)); //Important
	rigidBody_ = new btRigidBody(physicsAttribute->mass,
								 new btDefaultMotionState(a),
								 new btSphereShape(1),
								 btVector3(0,0,0));
	//rigidBody_->updateInertiaTensor();
	dynamicsWorld->addRigidBody(rigidBody_);
	forces_ = btVector3(0,0,0);
	rigidBody_->activate(true);

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

	btVector3 gravity(0,0,0);
	//btVector3 drag(linearVelocity*-100);
	rigidBody_->setWorldTransform(btTransform(rotation,position));
	rigidBody_->setLinearVelocity((linearVelocity));
	rigidBody_->setAngularVelocity(angularVelocity);
	rigidBody_->setMassProps(physicsAttribute->mass,btVector3(0,0,0));
	rigidBody_->setGravity(gravity+forces_);
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
	btQuaternion rotation = rigidBody_->getWorldTransform().getRotation();
	float yaw = asin(-2*(rotation.x()*rotation.z() - rotation.w()*rotation.y()));
	//btVector3 movement = 50*btVector3(inputAttribute->position[0], 0, inputAttribute->position[1]);
	btVector3 movement = 50*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	forces_ = movement.rotate(btVector3(0,1,0),yaw);	
}