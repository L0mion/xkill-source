#include "CollisionShapes.h"

#include <map>

#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <xkill-utilities/AttributeManager.h>

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
		if(collisionShapes_->at(i)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			for(int j = static_cast<btCompoundShape*>(collisionShapes_->at(i))->getNumChildShapes()-1; j > 0 ; j--)
			{
				delete static_cast<btCompoundShape*>(collisionShapes_->at(i))->getChildShape(j);
			}
		}
		delete collisionShapes_->at(i);
	}
	delete collisionShapes_;
	delete defaultShape_;
	delete frustrumShape_;
	importer_->deleteAllData();
	delete importer_;
}

btCollisionShape* CollisionShapes::getCollisionShape(unsigned int meshId)
{
	btCollisionShape* collisionShape;
	std::map<unsigned int, unsigned int>::iterator it = collisionShapesIdToIndex_.find(meshId);
	if(it != collisionShapesIdToIndex_.end())
	{
		collisionShape = collisionShapes_->at(it->second);
		return collisionShape;
	}
	else
	{
		return defaultShape_;
	}
}

void CollisionShapes::loadCollisionShapes()
{
	unloadCollisionShapes();

	//btTriangleMesh file load (memory leak, otherwise functioning)
	

	//while(itrMesh.hasNext())
	//{
	//	auto meshAttribute = itrMesh.getNext();

	//	//----------------------------------
	//	//-->check MEMORY LEAK
	//	btTriangleMesh *triangleMesh = new btTriangleMesh(); 
	//	//<--
	//	//----------------------------------

	//	auto vertices = meshAttribute->mesh.vertices_;
	//	for( unsigned int i = 0; i < meshAttribute->mesh.subsets_.size(); i++)
	//	{
	//		auto indices = meshAttribute->mesh.subsets_.at(i).indices_;
	//		for(unsigned int k = 0; k+2 < indices.size(); k+=3)
	//		{
	//			triangleMesh->addTriangle(btVector3(vertices[indices[k]].position_.x,
	//												vertices[indices[k]].position_.y,
	//												vertices[indices[k]].position_.z),
	//										btVector3(vertices[indices[k+1]].position_.x,
	//												vertices[indices[k+1]].position_.y,
	//												vertices[indices[k+1]].position_.z),
	//										btVector3(vertices[indices[k+2]].position_.x,
	//												vertices[indices[k+2]].position_.y,
	//												vertices[indices[k+2]].position_.z));
	//		}
	//	}

	//	if(meshAttribute->dynamic)
	//	{
	//		btConvexTriangleMeshShape tcs(triangleMesh);
	//	
	//		btShapeHull hull(&tcs);
	//		hull.buildHull(0);
	//		tcs.setUserPointer(&hull);
	//		btConvexHullShape* convexShape = new btConvexHullShape;
	//		for(int i=0; i< hull.numVertices(); i++)
	//		{
	//			convexShape->addPoint(hull.getVertexPointer()[i]);
	//		}

	//		collisionShapes_->push_back(convexShape);
	//		delete triangleMesh;
	//	}
	//	else
	//	{
	//		btBvhTriangleMeshShape* staticShape = new btBvhTriangleMeshShape(triangleMesh,true);
	//		collisionShapes_->push_back(staticShape);
	//		int a = staticShape->getShapeType();
	//		//triangleMeshes_push_back(triangleMesh);
	//	}

	//	unsigned int meshID		= meshAttribute->meshID;
	//	unsigned int meshIndex	= collisionShapes_->size() - 1;
	//	std::pair<unsigned int, unsigned int> idtoindex(meshID, meshIndex);
	//	collisionShapesIdToIndex_.insert(idtoindex);
	//}

	//.bullet file loading
	Settings* settings = ATTRIBUTE_MANAGER->settings;
	std::string filename = std::string("../../xkill-resources/xkill-level/");
	filename = filename.append(settings->currentLevel);
	filename = filename.append("/");
	filename = filename.append(settings->currentLevel);
	filename = filename.append(".bullet");
	SHOW_MESSAGEBOX("Blarg");
	if(!importer_->loadFile(filename.c_str()))
	{
		DEBUGPRINT("Level .bullet-file could not be loaded");
	}
	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		if(collisionShapesIdToIndex_.find(ptr_mesh->meshID) == collisionShapesIdToIndex_.end())
		{
			std::string name = ptr_mesh->fileName;
			name = name.substr(0,name.find("."));
			name = name.append("RigidBodyShape");
			btCollisionShape* collisionShape;
			btCollisionShape* loadedShape;
			loadedShape = importer_->getCollisionShapeByName(name.c_str());
			if(loadedShape != nullptr)
			{
				//REMOVE
				
				btVector3 scaling = loadedShape->getLocalScaling();
				collisionShape = new btBoxShape(scaling/2);
				collisionShape->setMargin(0.0f);
				collisionShape->setLocalScaling(btVector3(1,1,1));

				std::pair<unsigned int, unsigned int>  idToIndex(ptr_mesh->meshID,collisionShapes_->size());
				collisionShapesIdToIndex_.insert(idToIndex);
				btCompoundShape* compoundShape = new btCompoundShape();
				collisionShape->setMargin(0.0);
				
				compoundShape->addChildShape(importer_->getRigidBodyByName(name.c_str())->getWorldTransform(),collisionShape);
				compoundShape->setMargin(0.0);
				collisionShapes_->push_back(compoundShape);
				collisionShapes_->push_back(collisionShape);
			}
			else
			{
				//Load from model file
				filename = std::string("../../xkill-resources/xkill-models/");
				name = ptr_mesh->fileName;
				name = name.substr(0,name.find("."));
				filename = filename.append(name);
				filename = filename.append(".bullet");
				name = name.append("RigidBody");

				if(importer_->loadFile(filename.c_str()))
					loadedShape = importer_->getCollisionShapeByIndex(importer_->getNumCollisionShapes()-1);//name.c_str());
				if(loadedShape != nullptr)
				{
					//if(!name.compare("xkill_processRigidBody"))
					//{
					//	btBoxShape* box = static_cast<btBoxShape*>(loadedShape);
					//	btVector3 half = box->getHal fExtentsWithMargin();
					//	//btCapsuleShape* capsule = new btCapsuleShape( half.x() > half.z() ? half.x() : half.z(), half.y());
					//	//btSphereShape* sphere = new btSphereShape(0.2f);
					//	//collisionShape = capsule;
					//	//collisionShapes_->push_back(sphere);
					//	//collisionShapes_->push_back(capsule);
					//	//collisionShape = sphere;
					//}
					name = name.append("Shape");
					if(loadedShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
					{
						btVector3 scaling = loadedShape->getLocalScaling();
						collisionShape = new btBoxShape(scaling/2);
						collisionShape->setMargin(0.0f);
						collisionShape->setLocalScaling(btVector3(1,1,1));
						//collisionShape = new btBoxShape(*static_cast<btBoxShape*>(loadedShape));
					}
					else if(loadedShape->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
					{
						collisionShape = new btCapsuleShape(*static_cast<btCapsuleShape*>(loadedShape));
					}
					btCompoundShape* cs = new btCompoundShape();

					btRigidBody* rb =importer_->getRigidBodyByName(name.c_str());
					btTransform transform;
					if(rb!=nullptr)
					{
						transform = rb->getWorldTransform();
					}
					else
					{
						transform.setIdentity();
					}
					cs->addChildShape(transform,collisionShape);
					std::pair<unsigned int, unsigned int>  idToIndex(ptr_mesh->meshID,collisionShapes_->size());
					collisionShapesIdToIndex_.insert(idToIndex);
					collisionShapes_->push_back(collisionShape);
					cs->setMargin(0.00);
					collisionShapes_->push_back(cs);;
					
				}
			}
		}
	}
}

void CollisionShapes::unloadCollisionShapes()
{
	for(int i = 0; i < collisionShapes_->size(); i++)
	{
		if(collisionShapes_->at(i)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			for(int j = static_cast<btCompoundShape*>(collisionShapes_->at(i))->getNumChildShapes()-1; j > 0 ; j--)
			{
				delete static_cast<btCompoundShape*>(collisionShapes_->at(i))->getChildShape(j);
			}
		}
		delete collisionShapes_->at(i);
	}

	collisionShapes_->clear();
	collisionShapesIdToIndex_.clear();

	importer_->deleteAllData();
	delete importer_;
	importer_ = new btBulletWorldImporter();
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
	if(frustrumShape_ != nullptr)
	{
		delete frustrumShape_;
		frustrumShape_ = nullptr;
	}
	if(!itrCamera.count())
	{
		DEBUGPRINT("COLLISIONSHAPES: No cameras to use for frustum creation");
		return;
	}
	AttributePtr<Attribute_Camera> ptr_camera = itrCamera.at(0);
	float fovX = atan(ptr_camera->aspectRatio*tan(ptr_camera->fieldOfView));
	//cA->zFar =1;
	btVector3 points[8];
	points[0] = btVector3(-tan(fovX)*ptr_camera->zNear,	-tan(ptr_camera->fieldOfView)*ptr_camera->zNear, ptr_camera->zNear);
	points[1] = btVector3( tan(fovX)*ptr_camera->zNear,	-tan(ptr_camera->fieldOfView)*ptr_camera->zNear, ptr_camera->zNear);
	points[2] = btVector3(-tan(fovX)*ptr_camera->zNear,	 tan(ptr_camera->fieldOfView)*ptr_camera->zNear, ptr_camera->zNear);
	points[3] = btVector3( tan(fovX)*ptr_camera->zNear,	 tan(ptr_camera->fieldOfView)*ptr_camera->zNear, ptr_camera->zNear);
	points[4] = btVector3(-tan(fovX)*ptr_camera->zFar,	-tan(ptr_camera->fieldOfView)*ptr_camera->zFar,  ptr_camera->zFar);
	points[5] = btVector3( tan(fovX)*ptr_camera->zFar,	-tan(ptr_camera->fieldOfView)*ptr_camera->zFar,  ptr_camera->zFar);
	points[6] = btVector3(-tan(fovX)*ptr_camera->zFar,	 tan(ptr_camera->fieldOfView)*ptr_camera->zFar,  ptr_camera->zFar);
	points[7] = btVector3( tan(fovX)*ptr_camera->zFar,	 tan(ptr_camera->fieldOfView)*ptr_camera->zFar,  ptr_camera->zFar);

	btTriangleMesh* triangleMesh = new btTriangleMesh();
	triangleMesh->addTriangle(points[0],points[3],points[6]);
	triangleMesh->addTriangle(points[1],points[4],points[7]);
	triangleMesh->addTriangle(points[2],points[5],points[0]);
	btConvexTriangleMeshShape* tcs = new btConvexTriangleMeshShape(triangleMesh);
	btShapeHull* hull = new btShapeHull(tcs);
	hull->buildHull(0);
	tcs->setUserPointer(&hull);
	btConvexHullShape* convexShape = new btConvexHullShape();
	int numIndices = hull->numIndices();
	for(int i = 0; i < numIndices; i++)
	{
		int index = hull->getIndexPointer()[i];
		convexShape->addPoint(hull->getVertexPointer()[index]);
		btVector3 a = hull->getVertexPointer()[index];
		int b = 2;
	}
	frustrumShape_ = convexShape;

	delete hull;
	delete tcs;
	delete triangleMesh;
}

btCollisionShape* CollisionShapes::getFrustrumShape(unsigned int cameraIndex)
{
	//static btSphereShape a(1);	
	//return &a;
	return frustrumShape_;
}
