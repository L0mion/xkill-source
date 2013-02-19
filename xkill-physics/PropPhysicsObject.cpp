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
