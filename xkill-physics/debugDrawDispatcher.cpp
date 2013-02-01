#include "debugDrawDispatcher.h"

#include <xkill-utilities/Util.h>
#include <xkill-utilities/MeshVertices.h>

debugDrawDispatcher::debugDrawDispatcher()
{
	debugLineVertices = new std::vector<VertexPosColor>;
}

debugDrawDispatcher::~debugDrawDispatcher()
{
	delete debugLineVertices;
}

void debugDrawDispatcher::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	//Extract Bullet Physics lines (btVector3 points) into "VertexPosColor"
	VertexPosColor fromVertex = VertexPosColor(Float3(from.x(), from.y(), from.z()), Float3(color.x(), color.y(), color.z()));
	VertexPosColor toVertex = VertexPosColor(Float3(to.x(), to.y(), to.z()), Float3(color.x(), color.y(), color.z()));
	
	debugLineVertices->push_back(fromVertex);
	debugLineVertices->push_back(toVertex);
}

void debugDrawDispatcher::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{

}

void debugDrawDispatcher::reportErrorWarning(const char* warningString)
{
	DEBUGPRINT(warningString);
}

void debugDrawDispatcher::draw3dText(const btVector3& location,const char* textString)
{
	DEBUGPRINT("Trying to draw 3D text from debugDrawDispatcher.");
}

void debugDrawDispatcher::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int	debugDrawDispatcher::getDebugMode() const
{
	return m_debugMode;
}

void debugDrawDispatcher::clearDebugVerticesVector()
{
	debugLineVertices->clear();
}

void debugDrawDispatcher::queueDebugDrawEvent()
{
	if(debugLineVertices->size() > 0)
	{
		QUEUE_EVENT(new Event_DrawBulletPhysicsDebugLines(debugLineVertices));
	}
}