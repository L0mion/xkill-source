#ifndef XKILL_PHYSICS_MOTIONSTATE
#define XKILL_PHYSICS_MOTIONSTATE

#include <LinearMath/btMotionState.h>

class MotionState : public btMotionState
{
private:
public:
	MotionState();
	~MotionState();
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif