#include "CollisionShapeManager.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>

CollisionShapeManager::CollisionShapeManager()
{
}

CollisionShapeManager::~CollisionShapeManager()
{
	while(collisionShapes_.size()!=0)
	{
		delete collisionShapes_.at(collisionShapes_.size()-1);
		collisionShapes_.pop_back();
	}
}

btCollisionShape* CollisionShapeManager::getCollisionShape(unsigned int index)
{
	if(index < static_cast<unsigned int>(collisionShapes_.size()))
	{
		return collisionShapes_.at(index);
	}
	else
	{
		return nullptr;
	}
}

void CollisionShapeManager::createConvexHull(float* verticeData,unsigned int numVertices)
{
	btTriangleMesh trimesh;
	for(unsigned int i=0;i<numVertices;i+=9)
	{
		trimesh.addTriangle(btVector3(verticeData[i],   verticeData[i+1], verticeData[i+2]),
						    btVector3(verticeData[i+3], verticeData[i+4], verticeData[i+5]),
						    btVector3(verticeData[i+6], verticeData[i+7], verticeData[i+8]),
							true);
	}
	btConvexTriangleMeshShape tcs(&trimesh);
	btShapeHull hull(&tcs);
	hull.buildHull(0);
	tcs.setUserPointer(&hull);
	btConvexHullShape* convexShape = new btConvexHullShape;
	for(int i=0; i< hull.numVertices(); i++)
		convexShape->addPoint(hull.getVertexPointer()[i]);
	collisionShapes_.push_back(convexShape);
}