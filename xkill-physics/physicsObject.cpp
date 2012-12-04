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
	//memcpy(position,&positionAttribute->position,3*sizeof(float));
	position.setX(positionAttribute->position.x);
	position.setY(positionAttribute->position.y);
	position.setZ(positionAttribute->position.z);
	//memcpy(rotation,&spatialAttribute->rotation,4*sizeof(float));
	rotation.setX(0.0f);//spatialAttribute->rotation.x);
	rotation.setY(0.0f);//spatialAttribute->rotation.y);
	rotation.setZ(0.0f);//spatialAttribute->rotation.z);
	rotation.setW(1.0f);//spatialAttribute->rotation.w);
	//memcpy(linearVelocity,&physicsAttribute->linearVelocity,3*sizeof(float));
	linearVelocity.setX(physicsAttribute->linearVelocity.x);
	linearVelocity.setY(physicsAttribute->linearVelocity.y);
	linearVelocity.setZ(physicsAttribute->linearVelocity.z);
	//memcpy(angularVelocity,&physicsAttribute->angularVelocity,3*sizeof(float));
	angularVelocity.setX(physicsAttribute->angularVelocity.x);
	angularVelocity.setY(physicsAttribute->angularVelocity.y);
	angularVelocity.setZ(physicsAttribute->angularVelocity.z);

	//std::cout << "positionAttribute->position.x " << positionAttribute->position.x << std::endl;

	//std::cout << "position.x() " << position.x() << std::endl;

	//std::cout << "spatialAttribute->rotation.x " << spatialAttribute->rotation.x << std::endl;

	//std::cout << "physicsAttribute->linearVelocity.x " << physicsAttribute->linearVelocity.x << std::endl;

	//std::cout << "physicsAttribute->angularVelocity.x " << physicsAttribute->angularVelocity.x << std::endl;

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

	std::cout << "Position " << rigidBody_->getWorldTransform().getOrigin().getX() << std::endl;

	std::cout << "Rotation " << rigidBody_->getWorldTransform().getRotation().getX() << std::endl;


	////Convert btVector3 to Float3 and save in attributes
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