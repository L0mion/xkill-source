#include "CollisionShapeManager.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include "objLoaderBasic.h"

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
	ObjLoaderBasic loader;
	std::vector<VertexPosNormTex> vertices;
	loader.parseObjectFile("../../xkill-resources/xkill-models/bth.obj",&vertices);
	btTriangleMesh trimesh;
	for(unsigned int i=0;i<vertices.size();i+=3)
	{
		trimesh.addTriangle(btVector3(vertices.at(i).position_.x,   vertices.at(i).position_.y,   vertices.at(i).position_.z),
							btVector3(vertices.at(i+1).position_.x, vertices.at(i+1).position_.y, vertices.at(i+1).position_.z),
							btVector3(vertices.at(i+2).position_.x, vertices.at(i+2).position_.y, vertices.at(i+2).position_.z));
		/*trimesh.addTriangle(btVector3(verticeData[i],   verticeData[i+1], verticeData[i+2]),
						    btVector3(verticeData[i+3], verticeData[i+4], verticeData[i+5]),
						    btVector3(verticeData[i+6], verticeData[i+7], verticeData[i+8]),
							true);*/
	}
	btConvexTriangleMeshShape tcs(&trimesh);
	btShapeHull hull(&tcs);
	hull.buildHull(0);
	tcs.setUserPointer(&hull);
	btConvexHullShape* convexShape = new btConvexHullShape;
	for(int i=0; i< hull.numVertices(); i++)
	{
		btVector3 test = hull.getVertexPointer()[i];
		convexShape->addPoint(hull.getVertexPointer()[i]);
	}
	collisionShapes_.push_back(convexShape);
}

CollisionShapeManager* CollisionShapeManager::getInstance()
{
	static CollisionShapeManager instance;
	return &instance;
}