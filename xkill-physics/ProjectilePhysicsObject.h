#ifndef XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT
#define XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT

#include "PhysicsObject.h"

class ProjectilePhysicsObject
	: public PhysicsObject
{
public:
	ProjectilePhysicsObject();
	~ProjectilePhysicsObject();
	virtual bool subClassSpecificInitHook();
};

#endif