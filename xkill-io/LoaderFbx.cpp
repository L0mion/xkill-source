#include <sstream>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbx.h"
#include "LoaderFbxMesh.h"
#include "LoaderFbxMaterial.h"
#include "LoaderFbxAnimation.h"
#include "LoaderFbxMaterialDesc.h"

LoaderFbx::LoaderFbx()
{
	meshLoader_		= nullptr;
	materialLoader_ = nullptr;

	fbxManager_ = nullptr;
	fbxScene_	= nullptr;
}
LoaderFbx::~LoaderFbx()
{
	if(meshLoader_)
		delete meshLoader_;
	if(materialLoader_)
		delete materialLoader_;

	fbxManager_->Destroy();
}
bool LoaderFbx::init()
{
	meshLoader_		 = new LoaderFbxMesh();
	materialLoader_  = new LoaderFbxMaterial();
	materialDesc_	 = new LoaderFbxMaterialDesc();
	animationLoader_ = new LoaderFbxAnimation();

	bool success = true;
	success = createFbxManager();
	
	return success;
}

bool LoaderFbx::load(std::string filename)
{
	meshLoader_->reset();
	materialLoader_->reset();
	materialDesc_->reset();
	animationLoader_->reset();

	bool success = true;

	if(fbxScene_)
	{
		fbxScene_->Destroy();
		fbxScene_ = nullptr;
		success = createFbxScene();
	}
	else
		success = createFbxScene();
	if(success)
		success = loadScene(filename);

	FbxNode* node = fbxScene_->GetRootNode();
	if(node)
	{
		for(int i=0; i<node->GetChildCount(); i++)
			parseNode(node->GetChild(i));
	}

	animationLoader_->parseAnimation(fbxScene_);

	return success;
}

bool LoaderFbx::createFbxManager()
{
	bool success = true;

	fbxManager_ = FbxManager::Create();
	if(!fbxManager_)
	{
		success = false;
		SHOW_MESSAGEBOX("LoaderFbx::createFbxManager | fbxManager_ could not be created!");
	}
	else
	{
		FbxIOSettings* fbxIOSettings = FbxIOSettings::Create(fbxManager_, IOSROOT);
		fbxManager_->SetIOSettings(fbxIOSettings);

		FbxString path = FbxGetApplicationDirectory();
		fbxManager_->LoadPluginsDirectory(path.Buffer());
	}

	return success;
}
bool LoaderFbx::createFbxScene()
{
	bool success = true;

	fbxScene_ = FbxScene::Create(fbxManager_, "Scene 1");
	if(!fbxScene_)
	{
		success = false;
		SHOW_MESSAGEBOX("LoaderFbx::createFbxScene | fbxScene_ could not be created");
	}
	return success;
}
bool LoaderFbx::loadScene(std::string filename)
{
	bool success = true;

	int fileMajor, fileMinor, fileRevision;
	int sdkMajor, sdkMinor, sdkRevision;
	std::stringstream message;

	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager_, "");
	success = fbxImporter->Initialize(filename.c_str(), -1, fbxManager_->GetIOSettings());
	fbxImporter->GetFileVersion(fileMajor, fileMinor, fileRevision);

	if(!success)
	{
		message.str("");
		message << "LoaderFbx::loadScene | Call to FbxImporter::Initialize() failed! \n Error returned: " 
				<< fbxImporter->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());

		if(fbxImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedYet ||
		   fbxImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedAnymore)
		{
			std::stringstream message;
			message.str("");
			message << "FBX file format version for this FBX SDK is " << sdkMajor << "." << sdkMinor << "." << sdkRevision << "\n"
					<< "FBX file format version for file " << filename << " is " << fileMajor << "." << fileMinor << "." << fileRevision;
			SHOW_MESSAGEBOX(message.str());
		}
	}
	else
	{
		success = fbxImporter->Import(fbxScene_);
	}

	if(!success)
	{
		message.str("");
		message << "LoaderFbx::loadScene | Call to FbxImporter::Import() failed! \n Error returned: "
				<< fbxImporter->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	return success;
}

void LoaderFbx::parseNode(FbxNode* node)
{
	int errorCode = node->GetLastErrorID();
	if(errorCode != -1)
	{
		std::stringstream message;
		message << "LoaderFbx::parseNode | Error code : " << errorCode
				<< " Error message: " << node->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	FbxNodeAttribute::EType attributeType;

	if(node->GetNodeAttribute() == NULL)
		printf("LoaderFbx::parseNode | NULL Node Attribute \n\n");
	else
	{
		attributeType = node->GetNodeAttribute()->GetAttributeType();

		switch(attributeType)
		{
		case FbxNodeAttribute::eMesh:
			parseMesh(node);
			break;
		case FbxNodeAttribute::eSkeleton:
			break;
		}
	}

	for(int i=0; i<node->GetChildCount(); i++)
		parseNode(node->GetChild(i));
}
void LoaderFbx::parseMesh(FbxNode* node)
{
	FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
	meshLoader_->parseMesh(mesh);
	materialLoader_->parseMaterial(mesh);
}
void LoaderFbx::parseAnimation(FbxScene* scene)
{
	//for(int i=0; i<scene->GetSrcObjectCount<FbxAnimStack>(); i++)
	//{
	//	FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(i);
	//
	//	parseAnimationStack(animStack, scene->GetRootNode(), true);
	//	parseAnimationStack(animStack, scene->GetRootNode(), false);
	//}
}