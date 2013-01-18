#include "CollisionShapes.h"

#include <map>

#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/MeshModel.h>

AttributeIterator<Attribute_Mesh> itrMesh;

CollisionShapes::CollisionShapes()
{
	itrMesh	 = ATTRIBUTE_MANAGER->mesh.getIterator();
	collisionShapes_ = new btAlignedObjectArray<btCollisionShape*>();
	defaultShape_ = new btSphereShape(1);
	importer_ = new btBulletWorldImporter();
}

CollisionShapes::~CollisionShapes()
{
	for(int i = 0; i < collisionShapes_->size(); i++)
	{
		if(collisionShapes_->at(i)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE || collisionShapes_->at(i)->getShapeType() ==  SPHERE_SHAPE_PROXYTYPE )
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

				/*
				//REMOVE
				if(!name.compare("ThinWallRigidBodyShape"))
				{
					btBoxShape* box = (btBoxShape*)collisionShape;
					btVector3 half = box->getHalfExtentsWithMargin();
					//btCapsuleShape* capsule = new btCapsuleShape( half.x() > half.z() ? half.x() : half.z(), half.y());
					
					btTriangleMesh* triangleMesh = new btTriangleMesh();
					triangleMesh->addTriangle(btVector3(-1.0f, -2.5f, 0.0f), btVector3(1.0f, -2.5f, 0.0f), btVector3(-1.0f, 2.5f, 0.0f));
					triangleMesh->addTriangle(btVector3(1.0f, 2.5f, 0.0f), btVector3(1.0f, -2.5f, 0.0f), btVector3(-1.0f, 2.5f, 0.0f));
					btBvhTriangleMeshShape* BvhmeshShape = new btBvhTriangleMeshShape(triangleMesh,true);
					
					//collisionShape = capsule;
					//collisionShapes_->push_back(meshShape);
					collisionShape = BvhmeshShape;
				}
				*/


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
					//check compare (2013-01-17 17.19)
					if(name.compare("xkill_processRigidBody"))
					{
						btBoxShape* box = (btBoxShape*)collisionShape;
						btVector3 half = box->getHalfExtentsWithMargin();
						//btCapsuleShape* capsule = new btCapsuleShape( half.x() > half.z() ? half.x() : half.z(), half.y());
						btSphereShape* sphere = new btSphereShape(0.2);
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

