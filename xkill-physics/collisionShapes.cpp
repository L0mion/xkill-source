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
	triangleMeshDeallocation_ = new btAlignedObjectArray<btTriangleMesh*>();
	unusedCollisionShapes_ = new btAlignedObjectArray<btCollisionShape*>();
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

	for(int i=0;i<triangleMeshDeallocation_->size();i++)
	{
		delete triangleMeshDeallocation_->at(i);
	}
	delete triangleMeshDeallocation_;

	for(int i=0;i<unusedCollisionShapes_->size();i++)
	{
		delete unusedCollisionShapes_->at(i);
	}
	delete unusedCollisionShapes_;
	
	if(importer_)
	{
		importer_->deleteAllData();
		delete importer_;
		importer_ = nullptr;
	}

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
	
	DEBUGPRINT("-->Loading collision shapes...");

	std::string resourcePath = "../../xkill-resources/";
	//--------------------------------------------------------------------------------------
	// Load .bullet file corresponding with level pieces.
	//--------------------------------------------------------------------------------------
	
	Settings* settings = ATTRIBUTE_MANAGER->settings;
	std::string path = std::string(resourcePath + "xkill-level/") + settings->currentLevel + "/" + settings->currentLevel + ".bullet";
	DEBUGPRINT("Trying to load level .bullet file with path... " << path);
	if(importer_->loadFile(path.c_str())) //unknown chunk
	{
		DEBUGPRINT("...suceeded in loaded level .bullet file");
		findMappingBetweenMeshFilesAndBulletFiles();
		
		//Reallocate importer to prevent loading the level collision shapes twice
		if(importer_)
		{
			importer_->deleteAllData();
			delete importer_;
			importer_ = nullptr;
		}
		importer_ = new btBulletWorldImporter();

	}
	else //If no level .bullet file exists, load triangles as collision shape
	{
		DEBUGPRINT("...failed to load level .bullet file" << "\n\nThe level will be loaded with triangles as collision shapes.");
		while(itrMesh.hasNext())
		{
			AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
			if(ptr_mesh->meshID >=100) //Level pieces have mesh ids higher than 99
			{
				loadTrianglesFromMeshAsCollisionShape(ptr_mesh);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	// Load all .bullet files potentially corresponding to mesh files.
	// Assume "center1Corner.obj" corresponds to .bullet file "center1Corner.bullet".
	//--------------------------------------------------------------------------------------
	DEBUGPRINT("Mapping meshes with rigid bodies in .bullet files...");
	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		std::string meshName = ptr_mesh->fileName;
		meshName = meshName.substr(0,meshName.find("."));

		std::string path = std::string(resourcePath + "xkill-models/");
		path = path.append(meshName + ".bullet");
		if(importer_->loadFile(path.c_str()))
		{
			DEBUGPRINT("Assuming mapping between " + ptr_mesh->fileName + " and " + meshName + ".bullet");
		}
	}
	findMappingBetweenMeshFilesAndBulletFiles();
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

	//--------------------------------------------------------------------------------------
	// Create a compound shape with the created box as child shape.
	//--------------------------------------------------------------------------------------
	btCompoundShape* compoundShape = new btCompoundShape();
	compoundShape->addChildShape(*transform, collisionShape);
	compoundShape->setMargin(0.0);
	mapCollisionShapeToGlobalMeshId(meshId, compoundShape);

	unusedCollisionShapes_->push_back(collisionShape); //Add this to deallocation list
}

void CollisionShapes::loadTrianglesFromMeshAsCollisionShape(AttributePtr<Attribute_Mesh> ptr_mesh)
{
	btTriangleMesh* triangleMesh = new btTriangleMesh();

	std::vector<VertexDesc> vertices = ptr_mesh->mesh.vertices_;
	for(unsigned int i=0;i<ptr_mesh->mesh.subsets_.size();i++)
	{
		std::vector<unsigned int> indices = ptr_mesh->mesh.subsets_.at(i).indices_;
		for(unsigned int j=0;j+2<indices.size();j+=3)
		{
			btVector3 vertexA = btVector3(vertices[indices[j]].position_.x, vertices[indices[j]].position_.y, vertices[indices[j]].position_.z);
			btVector3 vertexB = btVector3(vertices[indices[j+1]].position_.x, vertices[indices[j+1]].position_.y, vertices[indices[j+1]].position_.z);
			btVector3 vertexC = btVector3(vertices[indices[j+2]].position_.x, vertices[indices[j+2]].position_.y, vertices[indices[j+2]].position_.z);

			triangleMesh->addTriangle(vertexA, vertexB, vertexC);
		}
	}
	if(ptr_mesh->dynamic)
	{
		btConvexTriangleMeshShape tcs(triangleMesh);

		btShapeHull hull(&tcs);
		hull.buildHull(0);
		//tcs.setUserPointer(&hull);
		btConvexHullShape* convexShape = new btConvexHullShape;
		for(int i=0; i< hull.numVertices(); i++)
		{
			convexShape->addPoint(hull.getVertexPointer()[i]);
		}

		mapCollisionShapeToGlobalMeshId(ptr_mesh->meshID, convexShape);
		delete triangleMesh;
	}
	else
	{
		btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape(triangleMesh, true);
		mapCollisionShapeToGlobalMeshId(ptr_mesh->meshID, triangleMeshShape);
		triangleMeshDeallocation_->push_back(triangleMesh);
	}
}

void CollisionShapes::mapCollisionShapeToGlobalMeshId(int meshId, btCollisionShape* collisionShape)
{
	std::pair<unsigned int, unsigned int> idToIndex(meshId, collisionShapes_->size());
	collisionShapesIdToIndex_.insert(idToIndex);

	collisionShapes_->push_back(collisionShape);
}

void CollisionShapes::findMappingBetweenMeshFilesAndBulletFiles()
{
	//--------------------------------------------------------------------------------------
	// Find mapping between mesh files and collision shapes contained in .bullet files.
	// Assume that a mesh file name of "center1Corner.obj" corresponds to a collision shape named "center1CornerRigidBodyShape".
	//--------------------------------------------------------------------------------------
	while(itrMesh.hasNext())
	{
		AttributePtr<Attribute_Mesh> ptr_mesh = itrMesh.getNext();
		
		std::string name = ptr_mesh->fileName;	//"center1Corner.obj"
		name = name.substr(0,name.find("."));	//"center1Corner"
		name = name.append("RigidBodyShape");	//"center1CornerRigidBodyShape"

		btRigidBody* rigidbody = importer_->getRigidBodyByName(name.c_str()); //Find rigid body inside .bullet file
		if(rigidbody != NULL)
		{
			btCollisionShape* loadedShape = rigidbody->getCollisionShape(); //Extract collision shape from rigid body found inside .bullet file
			if(loadedShape != NULL)
			{
				if(loadedShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
				{
					btVector3 scaling = loadedShape->getLocalScaling();			
					btTransform transform = rigidbody->getWorldTransform();
					addBoxCollisionShape(&scaling, &transform, ptr_mesh->meshID);
				}
				else
				{
					ERROR_MESSAGEBOX("CollisionShapes::findMappingBetweenMeshFilesAndBulletFiles, unsupported shape type found in rigid body " + name);
				}
			}
			else
			{
				loadTrianglesFromMeshAsCollisionShape(ptr_mesh);
				ERROR_MESSAGEBOX("CollisionShapes::findMappingBetweenMeshFilesAndBulletFiles, rigid body " + name + " has no collision shape.");
			}
		}
	}
}