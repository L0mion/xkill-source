#include "CollisionShapes.h"

#include <map>

#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <xkill-utilities/AttributeManager.h>

#include "physicsUtilities.h"

#include "physicsObject.h"

AttributeIterator<Attribute_Mesh> itrMesh;
AttributeIterator<Attribute_Camera> itrCamera;

CollisionShapes::CollisionShapes()
{
	itrMesh = ATTRIBUTE_MANAGER->mesh.getIterator();
	itrCamera = ATTRIBUTE_MANAGER->camera.getIterator();
	collisionShapes_ = new btAlignedObjectArray<btCollisionShape*>();
	defaultShape_ = new btSphereShape(btScalar(1.0f));
	//frustumShape_ = nullptr;
	importer_ = new btBulletWorldImporter();

	scatterProjectileCollisionShape = new btSphereShape(0.15f);

	float height = 0.75f;
	float radius = 0.25f;
	playerCollisionShape = new btCapsuleShape(radius, height);
}

CollisionShapes::~CollisionShapes()
{
	unloadCollisionShapes();

	delete collisionShapes_;
	delete defaultShape_;
	delete importer_;

	delete scatterProjectileCollisionShape;
	delete playerCollisionShape;
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
	importer_ = new btBulletWorldImporter();

	//btTriangleMesh file load (memory leak, otherwise functioning)
	
	//check lunch

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

	std::string resourcePath = "../../xkill-resources/";

	Settings* settings = ATTRIBUTE_MANAGER->settings;
	std::string path = std::string(resourcePath + "xkill-level/") + settings->currentLevel + "/" + settings->currentLevel + ".bullet";
	if(importer_->loadFile(path.c_str()))
	{
		DEBUGPRINT("Sucessfully loaded the .bullet file for " << path);
	}
	else
	{
		ERROR_MESSAGEBOX("Level .bullet-file could not be loaded.\nPath: " + path);
	}

	//check 2013-02-25 17.29-
	/*
	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		std::string path = std::string(resourcePath + "xkill-models/");
		path = path.append(ptr_mesh->fileName + ".bullet");
		importer_->loadFile(path.c_str());
	}

	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		int meshId = ptr_mesh->meshID;

		std::string name = ptr_mesh->fileName;
		name = name.substr(0,name.find("."));
		name = name.append("RigidBodyShape");
		btCollisionShape* loadedShape = importer_->getCollisionShapeByName(name.c_str()); //Find a collision shape in any .bullet file loaded by a "btBulletWorldImporter"
		
		if(loadedShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			btVector3 scaling = loadedShape->getLocalScaling();
						
			btTransform transform;
			btRigidBody* rigidbody = importer_->getRigidBodyByName(name.c_str());
			if(rigidbody != nullptr)
			{
				transform = rigidbody->getWorldTransform();
			}
			else
			{
				transform.setIdentity();
			}
			addBoxCollisionShape(&scaling, &transform, meshId);
		}
		else
		{
			ERROR_MESSAGEBOX("Unsupported shape type found");
		}
	}
	*/
	
	//--------------------------------------------------------------------------------------
	// Find mapping between mesh files and collision shapes contained in .bullet files
	// Assume that a mesh file name of "center1Corner.obj" corresponds to a collision shape named "center1CornerRigidBodyShape".
	//--------------------------------------------------------------------------------------
	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		int meshId = ptr_mesh->meshID;

		std::string name = ptr_mesh->fileName;
		name = name.substr(0,name.find("."));
		name = name.append("RigidBodyShape"); 
		btCollisionShape* loadedShape = importer_->getCollisionShapeByName(name.c_str()); //Find a collision shape in any .bullet file loaded by a "btBulletWorldImporter"
		//---------------------------------------------------------------------------------------
		// If a mapping is found: add box shape
		//--------------------------------------------------------------------------------------
		if(loadedShape != nullptr)
		{
			btVector3 scaling = loadedShape->getLocalScaling();
			btTransform transform = importer_->getRigidBodyByName(name.c_str())->getWorldTransform();
			addBoxCollisionShape(&scaling, &transform, meshId);
		}
		//--------------------------------------------------------------------------------------
		// Else: Load a new .bullet file and search for a rigid body. A rigid body is used for retrieving position and rotation (world transform).
		//--------------------------------------------------------------------------------------
		else
		{
			std::string path = std::string(resourcePath + "xkill-models/");
			std::string bulletFile;
			name = ptr_mesh->fileName;
			name = name.substr(0,name.find("."));
			path = path.append(name + ".bullet");
			bulletFile = path;
			name = name.append("RigidBody");

			//--------------------------------------------------------------------------------------
			// If .bullet file exists
			//--------------------------------------------------------------------------------------
			if(importer_->loadFile(path.c_str()))
			{
				loadedShape = importer_->getCollisionShapeByIndex(importer_->getNumCollisionShapes()-1);//name.c_str());
			}
			else
			{
				int g =5 ;
			}

			if(loadedShape != nullptr)
			{
				name = name.append("Shape");
				if(loadedShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
				{
					btVector3 scaling = loadedShape->getLocalScaling();
						
					btTransform transform;
					btRigidBody* rigidbody = importer_->getRigidBodyByName(name.c_str());
					if(rigidbody != nullptr)
					{
						transform = rigidbody->getWorldTransform();
					}
					else
					{
						transform.setIdentity();
					}
					addBoxCollisionShape(&scaling, &transform, meshId);
				}
				else
				{
					ERROR_MESSAGEBOX("Unsupported shape type found in " + bulletFile);
				}
			}
		}
	}
}

void CollisionShapes::unloadCollisionShapes()
{
	int nrOfChildShapes = collisionShapes_->size();
	for(int i = 0; i < nrOfChildShapes; i++)
	{
		delete collisionShapes_->at(i);
	}
	collisionShapes_->clear();

	collisionShapesIdToIndex_.clear();

	if(importer_)
	{
		importer_->deleteAllData();
		delete importer_;
		importer_ = nullptr;
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



void CollisionShapes::updateFrustumShape()
{	
	if(frustumShape_ != nullptr)
	{
		delete frustumShape_;
		frustumShape_ = nullptr;
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
	frustumShape_ = convexShape;

	delete hull;
	delete tcs;
	delete triangleMesh;
}

btCollisionShape* CollisionShapes::getFrustumShape(unsigned int cameraIndex)
{
	//static btSphereShape a(1);	
	//return &a;
	return frustumShape_;
}

void CollisionShapes::addBoxCollisionShape(btVector3* scaling, btTransform* transform, int meshId)
{
	btCollisionShape* collisionShape = new btBoxShape(*scaling/2);
	collisionShape->setMargin(0.0f);
	collisionShape->setLocalScaling(btVector3(1,1,1));

	mapInternalCollisionShapeIndexToGlobalMeshId(meshId);

	//--------------------------------------------------------------------------------------
	// Create a compound shape with the created box as child shape.
	//--------------------------------------------------------------------------------------
	btCompoundShape* compoundShape = new btCompoundShape();
	compoundShape->addChildShape(*transform, collisionShape);
	compoundShape->setMargin(0.0);
	collisionShapes_->push_back(compoundShape); //Add this for later access
	collisionShapes_->push_back(collisionShape); //Add this so that it can be deallocated easily
}

void CollisionShapes::mapInternalCollisionShapeIndexToGlobalMeshId(int meshId)
{
	std::pair<unsigned int, unsigned int> idToIndex(meshId, collisionShapes_->size());
	collisionShapesIdToIndex_.insert(idToIndex);
}
