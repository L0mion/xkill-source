#ifndef XKILL_PHYSICS_HOVERPHYSICSOBJECT
#define XKILL_PHYSICS_HOVERPHYSICSOBJECT

#include "PhysicsObject.h"

class HoverPhysicsObject
	: public PhysicsObject
{
private:
	btCollisionShape* localCollisionShape_;	//!< a players hoverboard, watchout mcfly
public:
	HoverPhysicsObject();
	~HoverPhysicsObject();
	virtual bool subClassSpecificInitHook();
	virtual void onUpdate(float delta,btDynamicsWorld* dynamicWorld);

};

#endif