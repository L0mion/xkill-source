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

btCollisionShape* CollisionShapeManager::getCollisionShape(unsigned int meshID)
{
	std::map<unsigned int, unsigned int>::iterator it = collisionShapesIDtoIndex_.find(meshID);
	if(it != collisionShapesIDtoIndex_.end())
	{
		unsigned int index = it->second;
		return collisionShapes_.at(index);
	}
	else
	{ //add collision shape
		return loadCollisionShape(meshID);
	}

	//loadCollisionShapes();
	//if(index < static_cast<unsigned int>(collisionShapes_.size()))
	//{
	//	return collisionShapes_.at(index);
	//}
	//else
	//{
	//	return collisionShapes_.at(0);
	//}
	
}

btCollisionShape* CollisionShapeManager::loadCollisionShape(unsigned int meshID)
{
	MeshAttribute* meshAttribute;
	bool foundMeshAt = false;
	for(unsigned int i = 0; i < meshAttributes_->size() && !foundMeshAt; i++)
	{
		if(meshAttributes_->at(i).meshID == meshID)
		{
			foundMeshAt = true;
			meshAttribute = &meshAttributes_->at(i);
		}
	}

	std::vector<VertexPosNormTex> vertices;
	
	btTriangleMesh *triangleMesh = new btTriangleMesh();;
	vertices = meshAttribute->mesh->getGeometry().getVertices();
	unsigned int numSubsets = meshAttribute->mesh->getGeometry().getNumSubsets();
	std::vector<unsigned int> indices;
	for(unsigned int j = 0; j < numSubsets; j++)
	{
		indices = meshAttribute->mesh->getGeometry().getSubsets().at(j).getIndices();
		unsigned int numIndices = indices.size();
		for(unsigned int k = 0; k+2 < numIndices; k+=3)
		{
			btScalar scale = 100.0f;
			btVector3 a = scale*btVector3(vertices[indices[k]].position_.x,
												vertices[indices[k]].position_.y,
												vertices[indices[k]].position_.z);
			triangleMesh->addTriangle(scale*btVector3(vertices[indices[k]].position_.x,
												vertices[indices[k]].position_.y,
												vertices[indices[k]].position_.z),
										scale*btVector3(vertices[indices[k+1]].position_.x,
												vertices[indices[k+1]].position_.y,
												vertices[indices[k+1]].position_.z),
										scale*btVector3(vertices[indices[k+2]].position_.x,
												vertices[indices[k+2]].position_.y,
												vertices[indices[k+2]].position_.z));
		}
	}

	if(meshAttribute->dynamic)
	{
		btConvexTriangleMeshShape tcs(triangleMesh);
		
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
		delete triangleMesh;
	}
	else
	{
		btBvhTriangleMeshShape* staticShape = new btBvhTriangleMeshShape(triangleMesh,true);
		collisionShapes_.push_back(staticShape);
		int a = staticShape->getShapeType();
		triangleMeshes_.push_back(triangleMesh);
		
	}

	unsigned int meshID		= meshAttribute->meshID;
	unsigned int meshIndex	= collisionShapes_.size() - 1;
	std::pair<unsigned int, unsigned int> idtoindex(meshID, meshIndex);
	collisionShapesIDtoIndex_.insert(idtoindex);

	return collisionShapes_.at(collisionShapes_.size() - 1);
}

void CollisionShapeManager::clean()
{
	while(collisionShapes_.size()!=0)
	{
		btCollisionShape* collisionShape = collisionShapes_.at(collisionShapes_.size()-1);
		int shapeType = collisionShape->getShapeType();
		delete collisionShape;
		collisionShapes_.pop_back();
	}
	while(triangleMeshes_.size()!=0)
	{
		btTriangleMesh* triangleMesh = triangleMeshes_.at(triangleMeshes_.size()-1);
		delete triangleMesh;
		triangleMeshes_.pop_back();
	}
};

void CollisionShapeManager::addCollisionShape(btCollisionShape* collisionShape)
{
	collisionShapes_.push_back(collisionShape);
}