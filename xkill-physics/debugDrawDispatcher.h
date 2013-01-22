#ifndef XKILL_PHYSICS_DEBUGDRAWDISPATCHER
#define XKILL_PHYSICS_DEBUGDRAWDISPATCHER

#include <LinearMath/btIDebugDraw.h>
#include <vector>

struct VertexPosColor;

class debugDrawDispatcher
	: public btIDebugDraw
{
private:
	int m_debugMode;

	std::vector<VertexPosColor>* debugLineVertices;

public:
	debugDrawDispatcher();
	virtual ~debugDrawDispatcher();

	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color); //! Called by Bullet Physics after call to btDiscreteDynamicsWorld::debugDrawWorld. Arguments sent by Bullet are the world space coordinates of all lines, as seen by Bullet, one at a time.
	
	//Management
	void clearDebugVerticesVector();
	void queueDebugDrawEvent();

	//Pure virtual overloads (refer to btIDebugDraw)
	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location,const char* textString);
	virtual void setDebugMode(int debugMode);
	virtual int	getDebugMode() const;

};

#endif