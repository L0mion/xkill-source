#include "physicsObject.h"

#include <xkill-utilities/AttributeType.h>
#include <vector>

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
	rigidBody_ = new btRigidBody(physicsAttribute->mass,
								 new btDefaultMotionState(a),
								 new btSphereShape(1),
								 btVector3(0,0,0));
	//rigidBody_->updateInertiaTensor();
	dynamicsWorld->addRigidBody(rigidBody_);
	forces_ = btVector3(0,0,0);
	movement_ = btVector3(0,0,0);
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

	//memcpy(position,positionAttribute->position,3*sizeof(float));
	//memcpy(rotation,spatialAttribute->rotation,4*sizeof(float));
	//memcpy(linearVelocity,physicsAttribute->linearVelocity,3*sizeof(float));
	//memcpy(angularVelocity,physicsAttribute->angularVelocity,3*sizeof(float));

	position = btVector3(positionAttribute->position[0],
						 positionAttribute->position[1],
						 positionAttribute->position[2]);
	rotation = btQuaternion(spatialAttribute->rotation[0],
							spatialAttribute->rotation[1],
							spatialAttribute->rotation[2],
							spatialAttribute->rotation[3]);
	linearVelocity = btVector3(physicsAttribute->linearVelocity[0],
							   physicsAttribute->linearVelocity[1],
							   physicsAttribute->linearVelocity[2]);
	angularVelocity = btVector3(physicsAttribute->angularVelocity[0],
							    physicsAttribute->angularVelocity[1],
							    physicsAttribute->angularVelocity[2]);

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

#include <iostream>
void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,spatialAttribute);

	//memcpy(positionAttribute->position,rigidBody_->getWorldTransform().getOrigin(),3*sizeof(float));
	//memcpy(spatialAttribute->rotation,rigidBody_->getWorldTransform().getRotation(),4*sizeof(float));
	//memcpy(physicsAttribute->linearVelocity,rigidBody_->getLinearVelocity(),3*sizeof(float));
	//memcpy(physicsAttribute->angularVelocity,rigidBody_->getAngularVelocity(),3*sizeof(float));

	positionAttribute->position[0] = rigidBody_->getCenterOfMassPosition().x();
	positionAttribute->position[1] = rigidBody_->getCenterOfMassPosition().y();
	positionAttribute->position[2] = rigidBody_->getCenterOfMassPosition().z();
	std::cout << positionAttribute->position[2];
	spatialAttribute->rotation[0] = rigidBody_->getWorldTransform().getRotation().x();
	spatialAttribute->rotation[1] = rigidBody_->getWorldTransform().getRotation().y();
	spatialAttribute->rotation[2] = rigidBody_->getWorldTransform().getRotation().z();
	spatialAttribute->rotation[3] = rigidBody_->getWorldTransform().getRotation().w();
	physicsAttribute->linearVelocity[0] = rigidBody_->getLinearVelocity().x();
	physicsAttribute->linearVelocity[1] = rigidBody_->getLinearVelocity().y();
	physicsAttribute->linearVelocity[2] = rigidBody_->getLinearVelocity().z();
	std::cout << physicsAttribute->linearVelocity[2];
	std::cout << "a";
	physicsAttribute->angularVelocity[0] = rigidBody_->getAngularVelocity().x();
	physicsAttribute->angularVelocity[1] = rigidBody_->getAngularVelocity().y();
	physicsAttribute->angularVelocity[2] = rigidBody_->getAngularVelocity().z();

	forces_ = btVector3(0,0,0);
}

void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	btQuaternion rotation = rigidBody_->getWorldTransform().getRotation();
	float yaw = asin(-2*(rotation.x()*rotation.z() - rotation.w()*rotation.y()));
	rotation = btQuaternion(0,yaw+inputAttribute->rotation[0],0);
	movement_ = btVector3(inputAttribute->position[0], 0, inputAttribute->position[1]);
	movement_ = 100*movement_.rotate(btVector3(0,1,0),yaw);
	//forces_ =movement_;
	inputAttribute->position[0] = inputAttribute->position[1] = 0;
}

