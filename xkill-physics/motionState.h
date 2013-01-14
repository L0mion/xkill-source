#ifndef XKILL_PHYSICS_MOTIONSTATE
#define XKILL_PHYSICS_MOTIONSTATE

#include <LinearMath/btMotionState.h>

class MotionState : public btMotionState
{
private:
	unsigned int attributeIndex_;
public:
	MotionState(unsigned int attributeIndex);
	~MotionState();
	void setAttributeIndex(unsigned int attributeIndex);
	unsigned int getAttributeIndex();
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif