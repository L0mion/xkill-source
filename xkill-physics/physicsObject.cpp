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
	preStep(physicsAttribute);
	dynamicsWorld->addRigidBody(rigidBody_);
}

void PhysicsObject::Clean(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(rigidBody_);
}



void PhysicsObject::preStep(PhysicsAttribute* physicsAttribute)
{
	
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatial = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, physicsAttribute);
	PositionAttribute* position = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatial);
	memcpy(&position->position, rigidBody_->getWorldTransform().getOrigin().get128().m128_f32, sizeof(float)*3);
}

#include <iostream>
void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	
	btVector3 force(inputAttribute->position.x,0,inputAttribute->position.y);
	btVector3 torque(0,inputAttribute->rotation.x,0);
	inputAttribute->position.y=inputAttribute->position.x = 0;
	rigidBody_->setGravity(btVector3(0,0,0)+100*force);
	//rigidBody_->applyTorque(torque);
	rigidBody_->applyDamping(delta*10);
	std::cout << "force:" << force.x() << " " << force.y() << " " << force.z() << std::endl;
	btVector3 f = rigidBody_->getTotalForce();
	rigidBody_->activate(true);
}