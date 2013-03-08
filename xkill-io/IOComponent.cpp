#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>

#include <DirectXMath.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include <xkill-utilities/AnimationClip.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"
#include "LoaderTexDesc.h"
#include "LoaderMdlDesc.h"
#include "MdlDesc.h"
#include "MdlDescModel.h"

#include "LoaderMD5.h"
#include "LoaderMD5AnimationDesc.h"
#include "LoaderMD5ModelDesc.h"
#include "LoaderMD5WeightDesc.h"

#include "LoaderFbx.h"
#include "LoaderFbxMeshDesc.h"
#include "LoaderFbxMaterialDesc.h"
#include "LoaderFbxModelDesc.h"
#include "LoaderFbxAnimationDesc.h"
#include "LoaderPGY.h"
#include "WriterPGY.h"

IOComponent::IOComponent()
{
	texNameToTexID_ = nullptr;
	fbxLoader_ = nullptr;

	SUBSCRIBE_TO_EVENT(this, EVENT_GET_FILE_LIST);
	SUBSCRIBE_TO_EVENT(this, EVENT_LOAD_LEVEL);
}
IOComponent::~IOComponent()
{
	if(texNameToTexID_)
		delete texNameToTexID_;

	if(fbxLoader_)
		delete fbxLoader_;

}
bool IOComponent::init()
{
	bool sucessfulInit = true;

	fbxLoader_ = new LoaderFbx();
	fbxLoader_->init();

	texNameToTexID_ = new std::map<std::string, unsigned int>();

	sucessfulInit = initTexDescs();
	if(sucessfulInit)
		sucessfulInit = initMdlDescs();	

	return sucessfulInit;
}

bool IOComponent::initTexDescs()
{
	bool sucessfulLoad = true;

	std::vector<std::string> texDescFiles;

	texDescFiles = getFileNames(PATH_TEXDESC, EXTENSION_TEXDESC);

	if(texDescFiles.size() > 0)
	{
		for(unsigned int i = 0; i < texDescFiles.size() && sucessfulLoad; i++)
			sucessfulLoad = initTexDesc(texDescFiles.at(i));
	}
	else
		ERROR_MESSAGEBOX("Couldn't locate any .texdesc-files in xkill-resources/.");

	return sucessfulLoad;
}
bool IOComponent::initTexDesc(std::string filename)
{
	bool sucessfulLoad = true;

	LoaderTexDesc* loader = new LoaderTexDesc(filename, PATH_XKILL_RESOURCES);
	sucessfulLoad = loader->init();
	
	if(sucessfulLoad)
	{
		TexDesc* texDesc = loader->claimTexDesc();

		/*Export me into seperate function without introducing any dependencies include-wize.*/
		std::vector<TexDescTex> textureDescriptions = texDesc->getTexDescs();
		for(unsigned int i = 0; i < textureDescriptions.size(); i++)
		{
			std::string texFilename	= textureDescriptions[i].fileName_;
			unsigned int texID		= textureDescriptions[i].id_; 

			std::pair<std::string, unsigned int> newMapping(texFilename, texID);
			texNameToTexID_->insert(newMapping);
		}

		Event_LoadTextures e(texDesc);
		SEND_EVENT(&e);
	}
	else
	{
		std::string errorMsg = "Failed to load .texdesc-file: " + filename;
		ERROR_MESSAGEBOX(errorMsg);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
}

bool IOComponent::initMdlDescs()
{
	bool sucessfulLoad = true;

	std::vector<std::string> mdlDescFiles;
	mdlDescFiles = getFileNames(PATH_MDLDESC, EXTENSION_MDLDESC);

	if(mdlDescFiles.size() > 0)
	{
		for(unsigned int i = 0; i < mdlDescFiles.size() && sucessfulLoad; i++)
			sucessfulLoad = initMdlDesc(mdlDescFiles.at(i));
	}
	else
	{
		ERROR_MESSAGEBOX("Couldn't locate any .mdldesc-files in xkill-resources/.");
	}
	//Settings* settings = ATTRIBUTE_MANAGER->settings;
	//sucessfulLoad = initLvlMdlDesc(settings->currentLevel);

	return sucessfulLoad;
}
bool IOComponent::initMdlDesc(std::string filename)
{
	bool sucessfulLoad = true;

	LoaderMdlDesc* loader = new LoaderMdlDesc(filename, PATH_XKILL_RESOURCES);
	sucessfulLoad = loader->init();
	
	if(sucessfulLoad)
	{
		MdlDesc* mdlDesc = loader->claimMdlDesc();
		std::vector<MdlDescModel*> models = mdlDesc->getModels();
		//loader->transferEventsToGame();

		std::string path = mdlDesc->getPath() + mdlDesc->getHeader().path_;
		for(unsigned int i = 0; i < models.size() && sucessfulLoad; i++)
		{
			std::string name = models[i]->modelFileName_;

			sucessfulLoad = loadModel(name, path, models[i]);
		}

		delete mdlDesc; //clear when finished
	}
	else
	{
		std::string errorMsg = "Failed to load .mdldesc-file: " + filename;
		ERROR_MESSAGEBOX(errorMsg);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
}

bool IOComponent::initLvlMdlDesc(std::string filename)
{
	bool sucessfulLoad = true;

	std::string path = PATH_XKILL_RESOURCES_LEVELS;
	path.append(filename);
	path.append("/");
	filename.append(".mdldesc");
	LoaderMdlDesc* loader = new LoaderMdlDesc(filename, path);

	DEBUGPRINT("Loading level... " + path);
	sucessfulLoad = loader->init();
	
	if(sucessfulLoad)
	{
		MdlDesc* mdlDesc = loader->claimMdlDesc();
		std::vector<MdlDescModel*> models = mdlDesc->getModels();
		loader->transferEventsToGame();

		std::string path = mdlDesc->getPath() + mdlDesc->getHeader().path_;
		for(unsigned int i = 0; i < models.size() && sucessfulLoad; i++)
		{
			std::string name = models[i]->modelFileName_;
			sucessfulLoad = loadModel(name, path, models[i]);
		}

		delete mdlDesc; //clear when finished
		DEBUGPRINT("...level loading completed");
	}
	else
	{
		std::string errorMsg = "Failed to load .mdldesc-file: " + filename;
		ERROR_MESSAGEBOX(errorMsg);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
}

bool IOComponent::loadModel(std::string	modelName, std::string	modelPath, MdlDescModel* modelDesc)
{
	bool successfulLoad = true;

	MeshDesc meshDesc;
	SkinnedData* skinnedData = nullptr;

	if(pollFile(modelPath, modelName + ".pgy"))
	{
		successfulLoad = loadPGY(modelName + ".pgy", modelPath, modelDesc, meshDesc, &skinnedData);
		//if(skinnedData)
		//	delete skinnedData;
	}
	else
	{
		skinnedData = new SkinnedData();
		FileExtension fileType = findFileType(modelName);
		switch(fileType)
		{
		case FILE_EXTENSION_FBX:
			successfulLoad = loadFbx(modelName, modelPath, modelDesc, meshDesc, skinnedData);
			break;
		case FILE_EXTENSION_OBJ:
			successfulLoad = loadObj(modelName, modelPath, modelDesc, meshDesc);
			break;
		case FILE_EXTENSION_MD5MESH:
			successfulLoad = loadMD5(modelName, modelPath, modelDesc, meshDesc, skinnedData);
			break;
		}

		writePGY(modelName + ".pgy", modelPath, meshDesc, (VertexType)modelDesc->vertexType_, skinnedData);
		
		//delete skinnedData;
	}

	if(skinnedData)
	{
		if(skinnedData->getAnimations()->size() > 0)
		{
			Event_AnimationLoaded animationEvent(modelDesc->modelID_, skinnedData);
			SEND_EVENT(&animationEvent);
		}
	}

	if(successfulLoad)
	{
		Event_CreateMesh e(
			modelDesc->modelID_, 
			meshDesc, 
			modelDesc->dynamic_, 
			modelName, 
			(VertexType)modelDesc->vertexType_);
		SEND_EVENT(&e);
	}
	else
	{
		std::string errorMsg = "Could not load model: " + modelPath + modelName;
		ERROR_MESSAGEBOX(errorMsg);
	}

	return successfulLoad;
}

bool IOComponent::loadObj(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc)
{
	bool sucessfulMake = true;

	MeshMakerObj* objMaker = new MeshMakerObj(
		modelPath, 
		modelPath, 
		modelName, 
		modelPath,
		texNameToTexID_);
	sucessfulMake = objMaker->init();

	if(sucessfulMake)
	{
		meshDesc = objMaker->getMesh();
	}
	else
	{
		sucessfulMake = false;
	}

	//if(sucessfulMake)
	//{
	//	model = objMaker->getMesh();
	//
	//	Event_CreateMesh e(modelDesc->modelID_, model, modelDesc->dynamic_);
	//	SEND_EVENT(&e);
	//}
	//else
	//{
	//	std::string errorMsg = "Could not load model: " + modelPath + modelName;
	//	ERROR_MESSAGEBOX(errorMsg);
	//}

	delete objMaker;
	return sucessfulMake;
}

bool IOComponent::loadFbx(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc, SkinnedData* skinnedData)
{
	bool successfulLoad = true;

	std::vector<LoaderFbxModelDesc> fbxModels = fbxLoader_->load(modelPath+modelName);
	
	if(fbxModels.size() > 0)
	{
		LoaderFbxMeshDesc mesh						= fbxModels[0].getMeshDesc();
		LoaderFbxMaterialDesc material				= fbxModels[0].getMaterialDesc();
		std::vector<LoaderFbxTextureDesc> textures	= fbxModels[0].getTextureDescs();

		loadFbxMesh(&mesh, &material, meshDesc, textures);
		loadFbxAnimation(fbxModels[0].getAnimationDescs(), fbxModels[0].getMeshDesc(), skinnedData);
	}
	else
		successfulLoad = false;

	return successfulLoad;
}
void IOComponent::loadFbxMesh(LoaderFbxMeshDesc* mesh, LoaderFbxMaterialDesc* material, MeshDesc& meshDesc, std::vector<LoaderFbxTextureDesc> texDescs)
{
	std::vector<VertexDesc> vertices;
	std::vector<unsigned int> indices;
	MaterialDesc materialDesc;

	mesh->createVertices(vertices, indices);
	materialDesc	= material->getMaterialDesc();

	if(texDescs.size() > 0)
		materialDesc.idAlbedoTex_ = getTexIDfromName(texDescs.at(0).getFileName());
	if(texDescs.size() > 1)
		materialDesc.idNormalTex_ = getTexIDfromName(texDescs.at(1).getFileName());

	std::vector<MaterialDesc> materials;
	materials.push_back(materialDesc);

	std::vector<SubsetDesc> subsets;
	SubsetDesc subset;
	subset.indices_ = indices;
	subset.materialIndex_ = 0;
	subsets.push_back(subset);

	meshDesc.vertices_ = vertices;
	meshDesc.materials_ = materials;
	meshDesc.subsets_ = subsets;
}
void IOComponent::loadFbxAnimation(std::vector<LoaderFbxAnimationDesc> animationDescs, LoaderFbxMeshDesc mesh, SkinnedData* skinnedData)
{
	std::map<std::string, AnimationClip*>* animations = new std::map<std::string, AnimationClip*>();

	for(unsigned int i=0; i<animationDescs.size(); i++)
	{
		animationDescs[i].convertToXKillFormat(animations);
	}

	std::vector<int>* boneHierarchy = new std::vector<int>();
	for(unsigned int i=0; i<mesh.getBoneParentIndices().size(); i++)
		boneHierarchy->push_back(mesh.getBoneParentIndices().at(i));
	std::vector<DirectX::XMFLOAT4X4>* boneOffsets = new std::vector<DirectX::XMFLOAT4X4>();
	for(unsigned int i=0; i<mesh.getOffsetMatrices().size(); i++)
	{
		DirectX::XMFLOAT4X4 matrix = DirectX::XMFLOAT4X4(
			mesh.getOffsetMatrices().at(i)._11, mesh.getOffsetMatrices().at(i)._12, mesh.getOffsetMatrices().at(i)._13, mesh.getOffsetMatrices().at(i)._14,
			mesh.getOffsetMatrices().at(i)._21, mesh.getOffsetMatrices().at(i)._22, mesh.getOffsetMatrices().at(i)._23, mesh.getOffsetMatrices().at(i)._24,
			mesh.getOffsetMatrices().at(i)._31, mesh.getOffsetMatrices().at(i)._32, mesh.getOffsetMatrices().at(i)._33, mesh.getOffsetMatrices().at(i)._34,
			mesh.getOffsetMatrices().at(i)._41, mesh.getOffsetMatrices().at(i)._42, mesh.getOffsetMatrices().at(i)._43, mesh.getOffsetMatrices().at(i)._44);
		boneOffsets->push_back(matrix);
	}

	skinnedData->set(boneHierarchy, boneOffsets, animations);
}

bool IOComponent::loadMD5(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc, SkinnedData* skinnedData)
{
	LoaderMD5 loaderMD5;
	LoaderMD5ModelDesc md5Model;
	loaderMD5.loadModel(modelPath+modelName, &md5Model);

	std::string animationName = loadMD5FindAnimationName(modelName);

	LoaderMD5AnimationDesc md5Animation;
	loaderMD5.loadAnimation(modelPath+ animationName, &md5Animation);

	std::vector<VertexDesc> vertices;
	std::vector<SubsetDesc> subsets;
	std::vector<MaterialDesc> materials;
	loadMD5AssembleVertices(&vertices, &md5Model);
	loadMD5AssembleSubsets(&subsets, &md5Model);
	loadMD5AssembleMaterials(&materials, &md5Model);
	meshDesc.vertices_	= vertices;
	meshDesc.subsets_	= subsets;
	meshDesc.materials_ = materials;

	loadMD5AssembleAnimation(skinnedData, &md5Animation, &md5Model);

	return true;
}
void IOComponent::loadMD5AssembleVertices(std::vector<VertexDesc>* vertices, LoaderMD5ModelDesc* md5Model)
{
	for(unsigned int meshIndex=0; meshIndex<md5Model->meshes_.size(); meshIndex++)
	{
		std::vector<LoaderMD5VertexDesc> md5Vertices = md5Model->meshes_[meshIndex].vertices_;
		std::vector<LoaderMD5WeightDesc> md5Weights = md5Model->meshes_[meshIndex].weights_;
		for(unsigned int vertexIndex=0; vertexIndex<md5Vertices.size(); vertexIndex++)
		{
			VertexDesc vertex;
			vertex.position_.x = md5Vertices[vertexIndex].position_.x;
			vertex.position_.y = md5Vertices[vertexIndex].position_.y;
			vertex.position_.z = md5Vertices[vertexIndex].position_.z;
	
			vertex.normal_.x = md5Vertices[vertexIndex].normal_.x;
			vertex.normal_.y = md5Vertices[vertexIndex].normal_.y;
			vertex.normal_.z = md5Vertices[vertexIndex].normal_.z;
	
			vertex.textureCoordinates_.x = md5Vertices[vertexIndex].texcoord_.x;
			vertex.textureCoordinates_.y = md5Vertices[vertexIndex].texcoord_.y;
			
			int numWeights = md5Vertices[vertexIndex].numWeights_;
			if(numWeights > 4)
				numWeights = 4;

			float weights[4] = {0};
			int startWeight = md5Vertices[vertexIndex].startWeight_;
			for(int i=0; i<numWeights; i++)
			{
				weights[i] = md5Weights[startWeight+i].bias_;
				vertex.boneIndices_[i] = md5Weights[startWeight+i].jointID_;
			}
			vertex.weights_.x = weights[0];
			vertex.weights_.y = weights[1];
			vertex.weights_.z = weights[2];
			
			vertices->push_back(vertex);
		}
	}
}
void IOComponent::loadMD5AssembleSubsets(std::vector<SubsetDesc>* subsets, LoaderMD5ModelDesc* md5Model)
{
	std::vector<unsigned int> indices;
	int numVertices = 0;
	for(unsigned int meshIndex=0; meshIndex<md5Model->meshes_.size(); meshIndex++)
	{
		indices.clear();
		std::vector<unsigned int> md5Indices = md5Model->meshes_[meshIndex].indices_;
		for(unsigned int i=0; i<md5Indices.size(); i++)
		{
			indices.push_back(md5Indices[i]+numVertices);
		}
		numVertices += md5Model->meshes_[meshIndex].vertices_.size();
		
		SubsetDesc subset;
		subset.indices_ = indices;
		subset.materialIndex_ = meshIndex;
		subsets->push_back(subset);
	}
}
void IOComponent::loadMD5AssembleMaterials(std::vector<MaterialDesc>* materials, LoaderMD5ModelDesc* md5Model)
{
	for(unsigned int meshIndex=0; meshIndex<md5Model->meshes_.size(); meshIndex++)
	{
		MaterialDesc material;

		std::string textureName = md5Model->meshes_[meshIndex].shader_;
		bool atExtension = false;
		std::string extension = "dds";
		int extensionIndex = 0;
		for(unsigned int i=0; i<textureName.size(); i++)
		{
			if(atExtension)
			{
				if(extensionIndex < extension.size())
				{
					textureName.at(i) = extension.at(extensionIndex);
					extensionIndex++;
				}
				else
					textureName.erase(i);
			}
			if(textureName.at(i) == '.')
				atExtension = true;
		}


		material.idAlbedoTex_ = getTexIDfromName(textureName);

		materials->push_back(material);
	}
}
void IOComponent::loadMD5AssembleAnimation(SkinnedData* skinnedData, LoaderMD5AnimationDesc* md5Animation, LoaderMD5ModelDesc* md5Model)
{
	AnimationClip* animationClip = new AnimationClip();
	std::vector<LoaderMD5JointInfo> jointInfos = md5Animation->jointInfos_;
	
	std::vector<int>* boneHierarchy = new std::vector<int>();
	for(unsigned int i=0; i<jointInfos.size(); i++)
		boneHierarchy->push_back(jointInfos[i].parentID_);

	std::vector<DirectX::XMFLOAT4X4>* boneOffsets = new std::vector<DirectX::XMFLOAT4X4>();
	for(unsigned int i=0; i<md5Model->boneOffsets_.size(); i++)
		boneOffsets->push_back(md5Model->boneOffsets_[i]);

	std::vector<LoaderMD5FrameSkeleton> frameSkeletons = md5Animation->skeletons_;
	animationClip->getBoneAnimations()->resize(jointInfos.size());
	for(unsigned int i=0; i<animationClip->getBoneAnimations()->size(); i++)
	{
		animationClip->getBoneAnimations()->at(i) = new BoneAnimation();
	}

	for(unsigned int frameIndex=0; frameIndex<frameSkeletons.size(); frameIndex++)
	{
		for(unsigned int jointIndex=0; jointIndex<frameSkeletons[frameIndex].joints_.size(); jointIndex++)
		{
			Keyframe* keyframe = new Keyframe();
			keyframe->rotationQuaternion = frameSkeletons[frameIndex].joints_[jointIndex].orientationQuaternion_;
			keyframe->translation = frameSkeletons[frameIndex].joints_[jointIndex].position_;
			keyframe->scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
			keyframe->timePosition = md5Animation->frameDuration_ * frameIndex;
			
			animationClip->getBoneAnimations()->at(jointIndex)->addKeyframe(keyframe);
		}
	}

	std::map<std::string, AnimationClip*>* animations = new std::map<std::string, AnimationClip*>();
	std::pair<std::string, AnimationClip*> animation("Default", animationClip);
	animations->insert(animation);

	skinnedData->set(boneHierarchy, boneOffsets, animations);
}
std::string IOComponent::loadMD5FindAnimationName(std::string modelName)
{
	bool atExtension = false;
	std::string animationName = modelName;
	std::string extension = "md5anim";
	unsigned int extensionIndex = 0;
	for(unsigned int i=0; i<animationName.size(); i++)
	{
		if(atExtension)
		{
			animationName.at(i) = extension.at(extensionIndex);
			extensionIndex++;
		}
		if(animationName.at(i) == '.')
			atExtension = true;
	}
	return animationName;
}


bool IOComponent::loadPGY(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc, SkinnedData** skinnedData)
{
	SkinnedData* tempSkinned = nullptr;
	bool sucessfulLoad = true;

	LoaderPGY pgyLoader(
		modelPath,
		modelName);
	sucessfulLoad = pgyLoader.init();

	if(sucessfulLoad)
	{
		meshDesc = pgyLoader.getMeshModel();
		tempSkinned = pgyLoader.getSkinnedData();
		//if(writeTimeUTC == pgyLoader.getWriteTimeUTC())
	}

	(*skinnedData) = tempSkinned;
	

	return sucessfulLoad;
}

bool IOComponent::writePGY(std::string modelName, std::string modelPath, MeshDesc meshDesc, VertexType vertexType, SkinnedData* skinnedData)
{
	WriterPGY pgyWriter(
		meshDesc,
		//writeTimeUTC, //fix this
		modelPath,
		modelName,
		vertexType,
		skinnedData);
	bool sucessfulWrite = pgyWriter.init();

	return sucessfulWrite;
}

FileExtension IOComponent::findFileType(std::string modelName)
{
	bool atExtension = false;
	std::string extension;
	for(unsigned int i=0; i<modelName.size(); i++)
	{
		if(atExtension)
			extension.push_back(modelName.at(i));
		if(modelName.at(i) == '.')
			atExtension = true;
	}

	FileExtension type;
	bool typeObj = true;
	bool typeFbx = true;

	std::string extensionFbx	 = "fbx";
	std::string extensionFBX	 = "FBX";
	std::string extensionObj	 = "obj";
	std::string extensionMD5mesh = "md5mesh";
	std::string extensionMD5anim = "md5anim";

	if(strcmp(extension.c_str(), extensionFbx.c_str()) == 0)
		type = FILE_EXTENSION_FBX;
	else if(strcmp(extension.c_str(), extensionFBX.c_str()) == 0)
		type = FILE_EXTENSION_FBX;
	else if(strcmp(extension.c_str(), extensionObj.c_str()) == 0)
		type = FILE_EXTENSION_OBJ;
	else if(strcmp(extension.c_str(), extensionMD5mesh.c_str()) == 0)
		type = FILE_EXTENSION_MD5MESH;
	else if(strcmp(extension.c_str(), extensionMD5anim.c_str()) == 0)
		type = FILE_EXTENSION_MD5ANIM;
	else
		type = FILE_EXTENSION_UNKNOWN;

	return type;
}

bool IOComponent::pollFile(std::string path, std::string fileName)
{
	std::string fullPathPGY = path + fileName; 

	std::ifstream ifile(fullPathPGY);
	return ifile.good();
}

std::vector<std::string> IOComponent::getFileNames(const LPCTSTR filepath, const LPCTSTR filename, bool recursiveSearch)
{
	std::vector<std::string> foundFiles;

	WIN32_FIND_DATA findFileData;
	HANDLE searchHandleWinAPI;
	std::wstring fullpath;

	//Go down in the folder hierarchy 

	if(recursiveSearch)
	{
		fullpath = filepath;
		fullpath.append(L"*");

		searchHandleWinAPI = FindFirstFile(
			fullpath.c_str(),
			&findFileData);

		if(searchHandleWinAPI != INVALID_HANDLE_VALUE)
		{
			do 
			{
				if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(wcscmp(findFileData.cFileName, L".") == 0 || wcscmp(findFileData.cFileName, L"..") == 0)
						continue;

					std::wstring path = filepath;

					path.append(findFileData.cFileName);
					path.append(L"/");

					std::vector<std::string> result = getFileNames(path.c_str(), filename, recursiveSearch);

					for(unsigned int i = 0; i < result.size(); i++)
					{
						foundFiles.push_back(result.at(i));
					}
				}

			} while(FindNextFile(searchHandleWinAPI, &findFileData));
			FindClose(searchHandleWinAPI);
		}

	}

	//Add files to array

	fullpath = filepath;
	fullpath.append(L"*");
	fullpath.append(filename);

	searchHandleWinAPI = FindFirstFile(
		fullpath.c_str(),
		&findFileData);

	if(searchHandleWinAPI != INVALID_HANDLE_VALUE)
	{
		do 
		{
			WCHAR* foundFile = findFileData.cFileName;

			//convert WCHAR* to char*
			char ch[260];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, foundFile, -1, ch, 260, &DefChar, NULL);
			
			foundFiles.push_back(std::string(ch));

		} while(FindNextFile(searchHandleWinAPI, &findFileData));
		FindClose(searchHandleWinAPI);
	}

	return foundFiles;
}

unsigned int IOComponent::getTexIDfromName(std::string texFilename)
{
	unsigned int texID = 0;

	std::map<std::string, unsigned int>::iterator it;
	it = texNameToTexID_->find(texFilename);
	if(it != texNameToTexID_->end())
		texID = it->second;

	return texID;
}

void IOComponent::reset()
{
}

void IOComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
		case EVENT_GET_FILE_LIST:
		{
			Event_GetFileList* getFileListEvent = static_cast<Event_GetFileList*>(e);

			std::wstring filepath;
			filepath.assign(getFileListEvent->filepath.begin(), getFileListEvent->filepath.end());

			std::wstring extension;
			extension.assign(getFileListEvent->extension.begin(), getFileListEvent->extension.end());

			std::wstring filename;
			filename.append(extension);

			getFileListEvent->filenames = getFileNames(filepath.c_str(), filename.c_str(), true);
			break;
		}
		case EVENT_LOAD_LEVEL:
		{
			Event_LoadLevel* loadLevelEvent = static_cast<Event_LoadLevel*>(e);

			SEND_EVENT(&Event(EVENT_UNLOAD_LEVEL));

			if(initLvlMdlDesc(loadLevelEvent->levelName))
			{
				SETTINGS->currentLevel = loadLevelEvent->levelName;
				SEND_EVENT(&Event(EVENT_LOAD_LEVEL_BULLET));
			}
			else
			{
				
			}

			break;
		}
		default:
			break;
	}
}