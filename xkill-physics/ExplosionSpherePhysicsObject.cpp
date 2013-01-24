#include "ExplosionSpherePhysicsObject.h"

//#include <BulletCollision/CollisionShapes/btCollisionShape.h>
//#include <BulletCollision/CollisionShapes/btSphereShape.h>

//#include "collisionShapes.h"

ExplosionSpherePhysicsObject::ExplosionSpherePhysicsObject()
	: PhysicsObject()
{
}

ExplosionSpherePhysicsObject::~ExplosionSpherePhysicsObject()
{
}

bool ExplosionSpherePhysicsObject::subClassSpecificInitHook()
{
	//setCollisionShape(CollisionShapes::Instance()->getCollisionShape(143250));
	
	
	
	//btCollisionShape* collisionShape = getCollisionShape();
	//collisionShape->setLocalScaling(btVector3(1.5f, 1.5f, 1.5f));
	
	//btSphereShape* sphere = static_cast<btSphereShape*>(collisionShape);
	//sphere->setUnscaledRadius(2.0f);

	return true;
}