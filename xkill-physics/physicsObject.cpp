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
	rigidBody_->updateInertiaTensor();
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

	//memcpy(position,positionAttribute->position,3*sizeof(float));
	//memcpy(rotation,spatialAttribute->rotation,4*sizeof(float));
	
	memcpy(position,&positionAttribute->position,3*sizeof(float));
	memcpy(rotation,&spatialAttribute->rotation,3*sizeof(float));

	memcpy(linearVelocity,physicsAttribute->linearVelocity,3*sizeof(float));
	memcpy(angularVelocity,physicsAttribute->angularVelocity,3*sizeof(float));

	btVector3 gravity(0,0,0);
	//btVector3 drag(linearVelocity*-100);

	rigidBody_->setWorldTransform(btTransform(rotation,position));
	rigidBody_->setLinearVelocity((linearVelocity*0.1f));
	rigidBody_->setAngularVelocity(angularVelocity);
	rigidBody_->setMassProps(physicsAttribute->mass,btVector3(0,0,0));
	rigidBody_->setGravity(gravity+forces_);
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,spatialAttribute);

	//memcpy(positionAttribute->position,rigidBody_->getWorldTransform().getOrigin(),3*sizeof(float));
	//memcpy(spatialAttribute->rotation,rigidBody_->getWorldTransform().getRotation(),3*sizeof(float));
	//memcpy(physicsAttribute->linearVelocity,rigidBody_->getLinearVelocity(),3*sizeof(float));
	//memcpy(physicsAttribute->angularVelocity,rigidBody_->getAngularVelocity(),3*sizeof(float));
	
	memcpy(&positionAttribute->position,rigidBody_->getWorldTransform().getOrigin(),3*sizeof(float));
	memcpy(&spatialAttribute->rotation,rigidBody_->getWorldTransform().getRotation(),3*sizeof(float));
	memcpy(physicsAttribute->linearVelocity,rigidBody_->getLinearVelocity(),3*sizeof(float));
	memcpy(physicsAttribute->angularVelocity,rigidBody_->getAngularVelocity(),3*sizeof(float));
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