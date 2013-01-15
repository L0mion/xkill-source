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
	levelName_ = "";
}

CollisionShapes::~CollisionShapes()
{
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
	
	while(itrMesh.hasNext())
	{
		Attribute_Mesh* meshAttribute = itrMesh.getNext();
		if(collisionShapesIdToIndex_.find(meshAttribute->meshID) == collisionShapesIdToIndex_.end())
		{
			std::string name = meshAttribute->mesh->getOrigins().getName();
			name = name.substr(0,name.find("Mesh"));
			name = name.append("RigidBody");
			//Edit world variable to reflect true path
			std::string filename = levelName_;
			filename = std::string("../../xkill-resources/xkill-levels/");
			filename = filename.append(levelName_);
			filename = filename.append("/");
			filename = filename.append(levelName_);
			filename = filename.append(".bullet");
			importer_->loadFile(filename.c_str());
			btCollisionShape* collisionShape = importer_->getCollisionShapeByName(name.c_str());
			if(collisionShape != nullptr)
			{
				std::pair<unsigned int, unsigned int>  idToIndex(meshAttribute->meshID,collisionShapes_->size());
				collisionShapesIdToIndex_.insert(idToIndex);
				collisionShapes_->push_back(collisionShape);
			}
			else
			{
				//Load from model file
				filename = std::string("../../xkill-resources/xkill-models/");
				filename = filename.append(name);
				filename = filename.append(".bullet");
				importer_->loadFile(filename.c_str());
				if(importer_->getNumCollisionShapes())
					collisionShape = importer_->getCollisionShapeByIndex(importer_->getNumCollisionShapes()-1);//name.c_str());
				if(collisionShape != nullptr)
				{
					std::pair<unsigned int, unsigned int>  idToIndex(meshAttribute->meshID,collisionShapes_->size());
					collisionShapesIdToIndex_.insert(idToIndex);
					collisionShapes_->push_back(collisionShape);
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

