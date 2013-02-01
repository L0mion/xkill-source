#ifndef XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT
#define XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT

#include "PhysicsObject.h"

class ExplosionSpherePhysicsObject
	: public PhysicsObject
{
private:
	btCollisionShape* localCollisionShape_;			//!< Not stored in "CollisionShapes", where other collision shapes are stored. Every ExplosionSpherePhysicsObject needs its own local collision shape because they expand over time.
	float explosionSphereFinalRadius_;				//!< Compare current radius stored in Attribute_ExplosionSphere against this value in the "onUpdate" function.
	float explosionSphereExpansionRate_;			//!< Add this value to the radius stored in Attribute_ExplosionSphere.
public:
	ExplosionSpherePhysicsObject();
	~ExplosionSpherePhysicsObject();
	virtual bool subClassSpecificInitHook();
	virtual void onUpdate(float delta);
};

#endif