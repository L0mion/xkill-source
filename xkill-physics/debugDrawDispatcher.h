#ifndef XKILL_PHYSICS_DEBUGDRAWDISPATCHER
#define XKILL_PHYSICS_DEBUGDRAWDISPATCHER

#include <LinearMath/btIDebugDraw.h>

class debugDrawDispatcher
	: public btIDebugDraw
{
private:
	debugDrawDispatcher();
	~debugDrawDispatcher();

public:
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
};

#endif