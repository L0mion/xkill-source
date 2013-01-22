#include "FrustumPhysicsObject.h"

//#include <BulletCollision/CollisionShapes/btCollisionShape.h>
//#include <BulletCollision/CollisionShapes/btSphereShape.h>

//#include "collisionShapes.h"

FrustumPhysicsObject::FrustumPhysicsObject()
	: PhysicsObject()
{
}

FrustumPhysicsObject::~FrustumPhysicsObject()
{
}

bool FrustumPhysicsObject::subClassSpecificInitHook()
{
	return true;
}