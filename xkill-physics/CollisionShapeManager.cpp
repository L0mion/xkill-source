#include "CollisionShapeManager.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include "objLoaderBasic.h"
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

CollisionShapeManager::CollisionShapeManager()
{
	GET_ATTRIBUTES(meshAttributes_, MeshAttribute, ATTRIBUTE_MESH);
	//if(collisionShapes_.size()==0)
		//collisionShapes_.push_back(new btSphereShape(50));
}

CollisionShapeManager::~CollisionShapeManager()
{
	clean();
}

btCollisionShape* CollisionShapeManager::getCollisionShape(unsigned int index)
{
	loadCollisionShapes();
	if(index >= static_cast<unsigned int>(collisionShapes_.size()))
	{
		return collisionShapes_.at(index);
	}
	else
	{
		return collisionShapes_.at(0);
	}
	
}

void CollisionShapeManager::loadCollisionShapes()
{
	MeshAttribute* meshAttribute;
	std::vector<VertexPosNormTex> vertices;
	for(unsigned int i = collisionShapes_.size(); i < meshAttributes_->size(); i++)
	{
		meshAttribute = &meshAttributes_->at(i);
		vertices = meshAttribute->mesh->getGeometry().getVertices();
		btTriangleMesh triangleMesh;
		for(unsigned int i = 0; i+2 < vertices.size();i+=3)
		{
			triangleMesh.addTriangle(btVector3(vertices.at(i).position_.x,   vertices.at(i).position_.y,   vertices.at(i).position_.z),
									 btVector3(vertices.at(i+1).position_.x, vertices.at(i+1).position_.y, vertices.at(i+1).position_.z),
									 btVector3(vertices.at(i+2).position_.x, vertices.at(i+2).position_.y, vertices.at(i+2).position_.z));
		}
		btConvexTriangleMeshShape tcs(&triangleMesh);
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
}

void CollisionShapeManager::clean()
{
	while(collisionShapes_.size()!=0)
	{
		delete collisionShapes_.at(collisionShapes_.size()-1);
		collisionShapes_.pop_back();
	}
};
