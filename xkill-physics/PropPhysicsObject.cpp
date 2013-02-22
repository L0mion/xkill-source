#include "PropPhysicsObject.h"

PropPhysicsObject::PropPhysicsObject()
{

}

PropPhysicsObject::~PropPhysicsObject()
{

}

btVector3 PropPhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

bool PropPhysicsObject::subClassSpecificInitHook()
{
	forceActivationState(DISABLE_DEACTIVATION);

	return true;
}