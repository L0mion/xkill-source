#ifndef XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT
#define XKILL_PHYSICS_PROJECTILEPHYSICSOBJECT

#include "PhysicsObject.h"

class ProjectilePhysicsObject
	: public PhysicsObject
{
private:
	bool localCollisionShape_;
public:
	ProjectilePhysicsObject();
	~ProjectilePhysicsObject();
	virtual bool subClassSpecificInitHook();
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual btCollisionShape* subClassSpecificCollisionShape();
};

#endif