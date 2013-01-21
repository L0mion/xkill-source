#include "CollisionShapes.h"

#include <map>

#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/MeshModel.h>

#include "physicsUtilities.h"

AttributeIterator<Attribute_Mesh> itrMesh;
AttributeIterator<Attribute_Camera> itrCamera;

CollisionShapes::CollisionShapes()
{
	itrMesh = ATTRIBUTE_MANAGER->mesh.getIterator();
	itrCamera = ATTRIBUTE_MANAGER->camera.getIterator();
	collisionShapes_ = new btAlignedObjectArray<btCollisionShape*>();
	defaultShape_ = new btSphereShape(btScalar(1.0f));
	frustrumShape_ = nullptr;
	importer_ = new btBulletWorldImporter();
}

CollisionShapes::~CollisionShapes()
{
	for(int i = 0; i < collisionShapes_->size(); i++)
	{
		if(collisionShapes_->at(i)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE || collisionShapes_->at(i)->getShapeType() == SPHERE_SHAPE_PROXYTYPE )
		{
			
			delete collisionShapes_->at(i);
		}
	}
	delete collisionShapes_;
	delete defaultShape_;
	importer_->deleteAllData();
	delete importer_;
}

btCollisionShape* CollisionShapes::getCollisionShape(unsigned int meshId)
{
	std::map<unsigned int, unsigned int>::iterator it = collisionShapesIdToIndex_.find(meshId);
	if(it != collisionShapesIdToIndex_.end())
	{
		return collisionShapes_->at(it->second);
	}
	else
	{
		return defaultShape_;
	}
}

void CollisionShapes::loadCollisionShapes()
{
	//btTriangleMesh file load (memory leak, otherwise functioning)
	/*
	Attribute_Mesh* meshAttribute;
	while(itrMesh.hasNext())
	{
		meshAttribute = itrMesh.getNext();
		std::vector<VertexPosNormTex> vertices;



		//----------------------------------
		//-->check MEMORY LEAK
		btTriangleMesh *triangleMesh = new btTriangleMesh(); 
		//<--
		//----------------------------------



		vertices = meshAttribute->mesh->getGeometry().getVertices();
		unsigned int numSubsets = meshAttribute->mesh->getGeometry().getNumSubsets();
		std::vector<unsigned int> indices;
		for(unsigned int j = 0; j < numSubsets; j++)
		{
			indices = meshAttribute->mesh->getGeometry().getSubsets().at(j).getIndices();
			unsigned int numIndices = indices.size();
			for(unsigned int k = 0; k+2 < numIndices; k+=3)
			{
				btVector3 a = btVector3(vertices[indices[k]].position_.x,
													vertices[indices[k]].position_.y,
													vertices[indices[k]].position_.z);
				triangleMesh->addTriangle(btVector3(vertices[indices[k]].position_.x,
													vertices[indices[k]].position_.y,
													vertices[indices[k]].position_.z),
											btVector3(vertices[indices[k+1]].position_.x,
													vertices[indices[k+1]].position_.y,
													vertices[indices[k+1]].position_.z),
											btVector3(vertices[indices[k+2]].position_.x,
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
				convexShape->addPoint(hull.getVertexPointer()[i]);
			}

			collisionShapes_->push_back(convexShape);
			delete triangleMesh;
		}
		else
		{
			btBvhTriangleMeshShape* staticShape = new btBvhTriangleMeshShape(triangleMesh,true);
			collisionShapes_->push_back(staticShape);
			int a = staticShape->getShapeType();
			//triangleMeshes_push_back(triangleMesh);
		}

		unsigned int meshID		= meshAttribute->meshID;
		unsigned int meshIndex	= collisionShapes_->size() - 1;
		std::pair<unsigned int, unsigned int> idtoindex(meshID, meshIndex);
		collisionShapesIdToIndex_.insert(idtoindex);
	}
	*/

	//.bullet file loading
	Settings* settings = ATTRIBUTE_MANAGER->settings;
	std::string filename = std::string("../../xkill-resources/xkill-level/");
	filename = filename.append(settings->currentLevel);
	filename = filename.append("/");
	filename = filename.append(settings->currentLevel);
	filename = filename.append(".bullet");
	if(!importer_->loadFile(filename.c_str()))
	{
		DEBUGPRINT("Level .bullet-file could not be loaded");
	}
	while(itrMesh.hasNext())
	{
		Attribute_Mesh* meshAttribute = itrMesh.getNext();
		if(collisionShapesIdToIndex_.find(meshAttribute->meshID) == collisionShapesIdToIndex_.end())
		{
			std::string name = meshAttribute->mesh->getOrigins().getName();
			name = name.substr(0,name.find("Mesh"));
			name = name.append("RigidBodyShape");
			btCollisionShape* collisionShape;
			collisionShape = importer_->getCollisionShapeByName(name.c_str());
			if(collisionShape != nullptr)
			{
				//REMOVE
				
				//if(!name.compare("ThinWallRigidBodyShape"))
				//{
				//	btBoxShape* box = (btBoxShape*)collisionShape;
				//	btVector3 half = box->getHalfExtentsWithMargin();
				//	//btCapsuleShape* capsule = new btCapsuleShape( half.x() > half.z() ? half.x() : half.z(), half.y());
				//	
				//	btTriangleMesh* triangleMesh = new btTriangleMesh();
				//	triangleMesh->addTriangle(btVector3(0.0f, -2.5f, -1.0f), btVector3(0.0f, -2.5f, 1.0f), btVector3(0.0f, 2.5f, -1.0f));
				//	triangleMesh->addTriangle(btVector3(0.0f, 2.5f, 1.0f), btVector3(0.0f,  2.5f, -1.0f), btVector3(0.0f, -2.5f, 1.0f));
				//	btBvhTriangleMeshShape* BvhmeshShape = new btBvhTriangleMeshShape(triangleMesh,true);
				//	
				//	//collisionShape = capsule;
				//	//collisionShapes_->push_back(meshShape);
				//	collisionShape = BvhmeshShape;
				//}
				
				


				std::pair<unsigned int, unsigned int>  idToIndex(meshAttribute->meshID,collisionShapes_->size());
				collisionShapesIdToIndex_.insert(idToIndex);
				btCompoundShape* compoundShape = new btCompoundShape();
				compoundShape->addChildShape(importer_->getRigidBodyByName(name.c_str())->getWorldTransform(),collisionShape);
				collisionShapes_->push_back(compoundShape);
				//collisionShapes_->push_back(collisionShape);
			}
			else
			{
				//Load from model file
				filename = std::string("../../xkill-resources/xkill-models/");
				name = meshAttribute->mesh->getOrigins().getName();
				name = name.substr(0,name.find("Mesh"));
				name = name.append("RigidBody");
				filename = filename.append(name);
				filename = filename.append(".bullet");
				
				if(importer_->loadFile(filename.c_str()))
					collisionShape = importer_->getCollisionShapeByIndex(importer_->getNumCollisionShapes()-1);//name.c_str());
				if(collisionShape != nullptr)
				{
					if(!name.compare("xkill_processRigidBody"))
					{
						btBoxShape* box = (btBoxShape*)collisionShape;
						btVector3 half = box->getHalfExtentsWithMargin();
						//btCapsuleShape* capsule = new btCapsuleShape( half.x() > half.z() ? half.x() : half.z(), half.y());
						btSphereShape* sphere = new btSphereShape(0.2f);
						//collisionShape = capsule;
						collisionShapes_->push_back(sphere);
						collisionShape = sphere;
					}
					name = name.append("Shape");
					btCompoundShape* cs = new btCompoundShape();
					cs->addChildShape(importer_->getRigidBodyByName(name.c_str())->getWorldTransform(),collisionShape);
					std::pair<unsigned int, unsigned int>  idToIndex(meshAttribute->meshID,collisionShapes_->size());
					collisionShapesIdToIndex_.insert(idToIndex);
					//collisionShapes_->push_back(collisionShape);
					collisionShapes_->push_back(cs);;
					
				}
			}
		}
	}
}

CollisionShapes* CollisionShapes::instance = nullptr;

CollisionShapes* CollisionShapes::Instance()
{
	if(instance == nullptr)
	{
		instance = new CollisionShapes();
	}
	return instance;
}



void CollisionShapes::updateFrustrumShape()
{
	if(frustrumShape_)
		delete frustrumShape_;
	if(!itrCamera.size())
	{
		DEBUGPRINT("COLLISIONSHAPES: No cameras to use for frustum creation");
		return;
	}
	Attribute_Camera* cA = itrCamera.at(0);
	float fovX = cA->aspectRatio*cA->fieldOfView;


	btVector3 points[8];
	points[0] = btVector3(-tan(fovX)*cA->zNear,-tan(cA->fieldOfView)*cA->zNear, cA->zNear);
	points[1] = btVector3( tan(fovX)*cA->zNear, -tan(cA->fieldOfView)*cA->zNear, cA->zNear);
	points[2] = btVector3(-tan(fovX)*cA->zNear, tan(cA->fieldOfView)*cA->zNear, cA->zNear);
	points[3] = btVector3( tan(fovX)*cA->zNear,  tan(cA->fieldOfView)*cA->zNear, cA->zNear);
	points[4] = btVector3(-tan(fovX)*cA->zFar, -tan(cA->fieldOfView)*cA->zFar,  cA->zFar);
	points[5] = btVector3( tan(fovX)*cA->zFar,  -tan(cA->fieldOfView)*cA->zFar,  cA->zFar);
	points[6] = btVector3(-tan(fovX)*cA->zFar,  tan(cA->fieldOfView)*cA->zFar,  cA->zFar);
	points[7] = btVector3( tan(fovX)*cA->zFar,   tan(cA->fieldOfView)*cA->zFar,  cA->zFar);

	btTriangleMesh triangleMesh;
	triangleMesh.addTriangle(points[0],points[3],points[6]);
	triangleMesh.addTriangle(points[1],points[4],points[7]);
	triangleMesh.addTriangle(points[2],points[5],points[0]);
	btConvexTriangleMeshShape tcs(&triangleMesh);
	btShapeHull hull(&tcs);
	hull.buildHull(0);
	tcs.setUserPointer(&hull);
	btConvexHullShape* convexShape = new btConvexHullShape();
	for(int i = 0; i < hull.numTriangles(); i++)
	{
		convexShape->addPoint(hull.getVertexPointer()[i]);
		btVector3 a = hull.getVertexPointer()[i];
		int b = 2;
	}
	frustrumShape_ = convexShape;
}

btCollisionShape* CollisionShapes::getFrustrumShape()
{
	return frustrumShape_;
}
