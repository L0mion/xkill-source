#include <sstream>

#include <fbxsdk.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbx.h"
#include "LoaderFbxMesh.h"
#include "LoaderFbxMeshDesc.h"
#include "LoaderFbxMaterial.h"
#include "LoaderFbxTexture.h"
#include "LoaderFbxTextureDesc.h"
#include "LoaderFbxAnimation.h"
#include "LoaderFbxMaterialDesc.h"
#include "LoaderFbxAnimationDesc.h"
#include "LoaderFbxModelDesc.h"

LoaderFbx::LoaderFbx()
{
	meshLoader_			= nullptr;
	materialLoader_		= nullptr;
	textureLoader_		= nullptr;
	animationLoader_	= nullptr;

	fbxManager_ = nullptr;
	fbxScene_	= nullptr;
}
LoaderFbx::~LoaderFbx()
{
	if(meshLoader_)
		delete meshLoader_;
	if(materialLoader_)
		delete materialLoader_;
	if(textureLoader_)
		delete textureLoader_;
	if(animationLoader_)
		delete animationLoader_;

	fbxManager_->Destroy();
}
bool LoaderFbx::init()
{
	meshLoader_		 = new LoaderFbxMesh();
	materialLoader_  = new LoaderFbxMaterial();
	textureLoader_	 = new LoaderFbxTexture();
 	animationLoader_ = new LoaderFbxAnimation();

	bool success = true;
	success = createFbxManager();
	
	return success;
}

std::vector<LoaderFbxModelDesc> LoaderFbx::load(std::string filename)
{
	meshLoader_->reset();
	materialLoader_->reset();
	textureLoader_->reset();
	animationLoader_->reset();

	modelDescs_.clear();

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


	for(unsigned int i=0; i<modelDescs_.size(); i++)
	{
		std::vector<LoaderFbxAnimationDesc>	animationDescs;
		animationLoader_->parseAnimation(fbxScene_, &animationDescs, modelDescs_[i].getMeshDesc().getBoneNodes());
		modelDescs_[i].setAnimationDescs(animationDescs);
	}
	return modelDescs_;
}

bool LoaderFbx::createFbxManager()
{
	bool success = true;

	fbxManager_ = FbxManager::Create();
	if(!fbxManager_)
	{
		success = false;
		ERROR_MESSAGEBOX("LoaderFbx::createFbxManager | fbxManager_ could not be created!");
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
		ERROR_MESSAGEBOX("LoaderFbx::createFbxScene | fbxScene_ could not be created");
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
				<< fbxImporter->GetStatus().GetErrorString();
		ERROR_MESSAGEBOX(message.str());

		if(fbxImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			std::stringstream message;
			message.str("");
			message << "FBX file format version for this FBX SDK is " << sdkMajor << "." << sdkMinor << "." << sdkRevision << "\n"
					<< "FBX file format version for file " << filename << " is " << fileMajor << "." << fileMinor << "." << fileRevision;
			ERROR_MESSAGEBOX(message.str());
		}
	}
	if(fbxImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", filename, fileMajor, fileMinor, fileRevision);

        // From this point, it is possible to access animation stack information without
        // the expense of loading the entire file.

        FBXSDK_printf("Animation Stack Information\n");

        int animStackCount = fbxImporter->GetAnimStackCount();

        FBXSDK_printf("    Number of Animation Stacks: %d\n", animStackCount);
        FBXSDK_printf("    Current Animation Stack: \"%s\"\n", fbxImporter->GetActiveAnimStackName().Buffer());
        FBXSDK_printf("\n");

        for(int i = 0; i < animStackCount; i++)
        {
            FbxTakeInfo* takeInfo = fbxImporter->GetTakeInfo(i);

            FBXSDK_printf("    Animation Stack %d\n", i);
            FBXSDK_printf("         Name: \"%s\"\n", takeInfo->mName.Buffer());
			const char* debug = takeInfo->mDescription.Buffer();
	//		FBXSDK_printf("         Description: \"%s\"\n", takeInfo->mDescription.Buffer());
            
			// Change the value of the import name if the animation stack should be imported 
            // under a different name.
            FBXSDK_printf("         Import Name: \"%s\"\n", takeInfo->mImportName.Buffer());

            // Set the value of the import state to false if the animation stack should be not
            // be imported. 
            FBXSDK_printf("         Import State: %s\n", takeInfo->mSelect ? "true" : "false");
            FBXSDK_printf("\n");

			
        }

        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL,        true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE,         true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_LINK,            true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE,           true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO,            true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,       true);
        fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
	
	if(success)
	{
		success = fbxImporter->Import(fbxScene_);
	}

	if(!success)
	{
		message.str("");
		message << "LoaderFbx::loadScene | Call to FbxImporter::Import() failed! \n Error returned: "
				<< fbxImporter->GetStatus().GetErrorString();
		ERROR_MESSAGEBOX(message.str());
	}

	return success;
}

void LoaderFbx::parseNode(FbxNode* node)
{
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
	modelDescs_.push_back(LoaderFbxModelDesc());

	LoaderFbxMeshDesc					meshDesc;
	LoaderFbxMaterialDesc				materialDesc;
	std::vector<LoaderFbxTextureDesc>	textureDesc;

	FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
	meshLoader_->parseMesh(mesh, &meshDesc);
	materialLoader_->parseMaterial(mesh, &materialDesc);
	textureLoader_->parseTexture(mesh, &textureDesc);

	modelDescs_.back().setMeshDesc(meshDesc);
	modelDescs_.back().setMaterialDesc(materialDesc);
	modelDescs_.back().setTextureDesc(textureDesc);
}