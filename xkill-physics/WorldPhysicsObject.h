#ifndef XKILL_PHYSICS_WORLDPHYSICSOBJECT
#define XKILL_PHYSICS_WORLDPHYSICSOBJECT

#include "PhysicsObject.h"

class WorldPhysicsObject
	: public PhysicsObject
{
public:
	WorldPhysicsObject();
	~WorldPhysicsObject();
	virtual bool subClassSpecificInitHook();
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual void handleOutOfBounds();
};

#endif