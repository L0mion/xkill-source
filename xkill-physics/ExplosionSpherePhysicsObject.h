#ifndef XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT
#define XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT

#include "PhysicsObject.h"

class ExplosionSpherePhysicsObject
	: public PhysicsObject
{
private:
	btCollisionShape* localCopyOfCollisionShape_; //!< Not stored in "CollisionShapes", where other collision shapes are stored.
	float radius;
public:
	ExplosionSpherePhysicsObject();
	~ExplosionSpherePhysicsObject();
	virtual bool subClassSpecificInitHook();
	virtual void onUpdate(float delta);
};

#endif