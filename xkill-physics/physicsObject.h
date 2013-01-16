#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>

class PhysicsObject
	: public btRigidBody
{
	virtual bool subClassSpecificInitHook();	//! Can be overridden by subclasses. Is called from init.
protected:
		unsigned int attributeIndex_; //!< Specifies which PhysicsObject is synchronized with which physics attribute.
public:
	PhysicsObject();
	virtual ~PhysicsObject();
	//! Init PhysicsObject and call subClassSpecificInitHook()
	/*!
	\param attributeIndex The physics attribute index. Refer to MotionState.
	*/
	bool init(unsigned int attributeIndex);
	unsigned int getAttributeIndex() const;
	
	virtual void onUpdate(float delta);
};

#endif