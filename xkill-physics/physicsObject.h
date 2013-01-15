#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>

class PhysicsObject
	: public btRigidBody
{
private:
	virtual bool subClassSpecificInitHook();	//! Can be overridden by subclasses. Is called from init.
protected:
		float yaw_;
public:
	PhysicsObject();
	~PhysicsObject();
	//! Init PhysicsObject and call subClassSpecificInitHook()
	/*!
	\param attributeIndex The physics attribute index. Refer to MotionState.
	*/
	bool init(unsigned int attributeIndex);
	
	virtual void onUpdate(float delta);
};

#endif