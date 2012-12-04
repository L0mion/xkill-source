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
	
	//Copy from attributes to Bullet Physics internal representation
	memcpy(position,&positionAttribute->position,3*sizeof(float));
	memcpy(rotation,&spatialAttribute->rotation,4*sizeof(float));
	memcpy(linearVelocity,&physicsAttribute->linearVelocity,3*sizeof(float));
	memcpy(angularVelocity,&physicsAttribute->angularVelocity,3*sizeof(float));

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
	positionAttribute->position.copy(static_cast<float*>(rigidBody_->getWorldTransform().getOrigin()));
	spatialAttribute->rotation.copy(static_cast<float*>(rigidBody_->getWorldTransform().getRotation()));

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