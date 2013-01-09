#include "physicsObject.h"

#include <btBulletDynamicsCommon.h>

PhysicsObject::PhysicsObject() : btRigidBody(0, nullptr, nullptr)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::init(unsigned int attributeIndex)
{
	//Read all attribute data
	attributeIndex_ = attributeIndex;
}

void PhysicsObject::preStep()
{
	//Read frame vital attribute data
}

void PhysicsObject::postStep()
{
	//Write frame vital attribute data
}