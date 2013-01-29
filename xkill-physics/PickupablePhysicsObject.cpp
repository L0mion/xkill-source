#include "PickupablePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

PickupablePhysicsObject::PickupablePhysicsObject()
	: PhysicsObject()
{
}

PickupablePhysicsObject::~PickupablePhysicsObject()
{
}

bool PickupablePhysicsObject::subClassSpecificInitHook()
{
	return true;
}

btVector3 PickupablePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}