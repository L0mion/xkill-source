#include "ProjectilePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

//#include <xkill-utilities/AttributeManager.h>
//AttributeIterator<Attribute_Physics> itrPhysics_ProjectilePhysicsObject;

ProjectilePhysicsObject::ProjectilePhysicsObject()
	: PhysicsObject()
{
	//itrPhysics_ProjectilePhysicsObject = ATTRIBUTE_MANAGER->physics.getIterator();
}

ProjectilePhysicsObject::~ProjectilePhysicsObject()
{
}

bool ProjectilePhysicsObject::subClassSpecificInitHook()
{
	//Attribute_Physics* physicsAttribute = itrPhysics_ProjectilePhysicsObject.at(attributeIndex_);

	btVector3 velocity = getLinearVelocity();
	float speed = velocity.length();

	//Anti-tunneling
	setCcdMotionThreshold(1.0f);
	setCcdSweptSphereRadius(0.2f);

	return true;
}

btVector3 ProjectilePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}