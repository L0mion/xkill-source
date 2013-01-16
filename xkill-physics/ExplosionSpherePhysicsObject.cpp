#include "ExplosionSpherePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>


ExplosionSpherePhysicsObject::ExplosionSpherePhysicsObject()
	: PhysicsObject()
{
}

ExplosionSpherePhysicsObject::~ExplosionSpherePhysicsObject()
{
}

bool ExplosionSpherePhysicsObject::subClassSpecificInitHook()
{
	btCollisionShape* collisionShape = getCollisionShape();
	//collisionShape->
	//Continue
	return true;
}