#include "PickupablePhysicsObject.h"
#include "btBulletDynamicsCommon.h"

#include <xkill-utilities\Util.h>//check temp
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

void PickupablePhysicsObject::onUpdate(float delta)
{
	hover(delta, 1.0f);
	setLinearVelocity(btVector3(0.0f, getLinearVelocity().y(), 0.0f)); //Prevent pickupable from floating away
	setAngularVelocity(getAngularVelocity().absolute()*0.9f); //Gradually slow down spinning
}