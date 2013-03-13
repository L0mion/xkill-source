#ifndef XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT
#define XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT

#include "PhysicsObject.h"

class ProjectilePhysicsObject
	: public PhysicsObject
{
public:
	ProjectilePhysicsObject();
	~ProjectilePhysicsObject();
private:
	virtual bool subClassSpecificInitHook();
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual btCollisionShape* subClassSpecificCollisionShape();
	virtual void handleOutOfBounds();
	virtual void onUpdate();
};

#endif