#ifndef XKILL_PHYSICS_PROPPHYSICSOBJECT
#define XKILL_PHYSICS_PROPPHYSICSOBJECT

#include "physicsObject.h"

class PropPhysicsObject : public PhysicsObject
{
public:
	PropPhysicsObject();
	~PropPhysicsObject();

	btVector3 worldOrigin_;

	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual bool subClassSpecificInitHook();
	virtual void handleOutOfBounds();

	virtual std::string getName() {return "PropPhysicsObject";}
};

#endif