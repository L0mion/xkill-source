#ifndef XKILL_PHYSICS_MOTIONSTATE
#define XKILL_PHYSICS_MOTIONSTATE

#include <LinearMath/btMotionState.h>

//! Each PhysicsObject has a MotionState. It handles world transforms and binding between physics attributes and PhysicsObjects.
class MotionState : public btMotionState
{
private:
	unsigned int attributeIndex_; //!< Specifies which PhysicsObject is synchronized with which physics attribute.
	
public:
	MotionState();
	~MotionState();
	void setAttributeIndex(unsigned int attributeIndex);
	unsigned int getAttributeIndex();
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif