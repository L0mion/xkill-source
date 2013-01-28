#ifndef XKILL_PHYSICS_MOTIONSTATE
#define XKILL_PHYSICS_MOTIONSTATE

#include <LinearMath/btMotionState.h>

//! Each PhysicsObject has a MotionState. It handles world transforms.
class MotionState : public btMotionState
{
private:
	unsigned int attributeIndex_; //!< Specifies which PhysicsObject is synchronized with which physics attribute.
	
public:
	MotionState(unsigned int attributeIndex); 
	~MotionState();
	virtual void getWorldTransform(btTransform &worldTrans) const;  //!< Gives bullet the position and spatial of an entity
	virtual void setWorldTransform(const btTransform &worldTrans);  //!< Bullet sets the position and spatial of an entity
};

#endif