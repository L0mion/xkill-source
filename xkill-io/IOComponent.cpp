#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"
#include "LoaderTexDesc.h"
#include "LoaderMdlDesc.h"
#include "MdlDesc.h"
#include "MdlDescModel.h"

#include "LoaderFbx.h"
#include "LoaderFbxModelDesc.h"
#include "LoaderFbxAnimationDesc.h"
#include "LoaderPGY.h"
#include "WriterPGY.h"

IOComponent::IOComponent()
{
	texNameToTexID = nullptr;
	fbxLoader_ = nullptr;
}
IOComponent::~IOComponent()
{
	if(texNameToTexID)
		delete texNameToTexID;

	if(fbxLoader_)
		delete fbxLoader_;

}
bool IOComponent::init()
{
	bool sucessfulInit = true;

	fbxLoader_ = new LoaderFbx();
	fbxLoader_->init();

	texNameToTexID = new std::map<std::string, unsigned int>();

	sucessfulInit = initTexDescs();
	if(sucessfulInit)
		sucessfulInit = initMdlDescs();	

	return sucessfulInit;
}

bool IOComponent::initTexDescs()
{
	bool sucessfulLoad = true;

	std::vector<std::string> texDescFiles;

	texDescFiles = getFileNames(PATH_TEXDESC);

	if(texDescFiles.size() > 0)
	{
		for(unsigned int i = 0; i < texDescFiles.size() && sucessfulLoad; i++)
			sucessfulLoad = initTexDesc(texDescFiles.at(i));
	}
	else
		SHOW_MESSAGEBOX("Couldn't locate any .texdesc-files in xkill-resources/.");

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
			texNameToTexID->insert(newMapping);
		}

		Event_LoadTextures e(texDesc);
		SEND_EVENT(&e);
	}
	else
	{
		std::string errorMsg = "Failed to load .texdesc-file: " + filename;
		SHOW_MESSAGEBOX(errorMsg);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
}

bool IOComponent::initMdlDescs()
{
	bool sucessfulLoad = true;

	std::vector<std::string> mdlDescFiles;
	mdlDescFiles = getFileNames(PATH_MDLDESC);

	if(mdlDescFiles.size() > 0)
	{
		for(unsigned int i = 0; i < mdlDescFiles.size() && sucessfulLoad; i++)
			sucessfulLoad = initMdlDesc(mdlDescFiles.at(i));
	}
	else
	{
		SHOW_MESSAGEBOX("Couldn't locate any .mdldesc-files in xkill-resources/.");
	}
	Settings* settings = ATTRIBUTE_MANAGER->settings;
	sucessfulLoad = initLvlMdlDesc(settings->currentLevel);

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
		SHOW_MESSAGEBOX(errorMsg);
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
	}
	else
	{
		std::string errorMsg = "Failed to load .mdldesc-file: " + filename;
		SHOW_MESSAGEBOX(errorMsg);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
}

bool IOComponent::loadModel(
	std::string		modelName,
	std::string		modelPath,
	MdlDescModel*	modelDesc)
{
	bool successfulLoad = true;

	MeshDesc meshDesc;
	if(pollFile(modelPath, modelName + ".pgy"))
	{
		successfulLoad = loadPGY(modelName + ".pgy", modelPath, modelDesc, meshDesc);
	}
	else
	{
		FileExtension fileType = findFileType(modelName);
		switch(fileType)
		{
		case FILE_EXTENSION_FBX:
			successfulLoad = loadFbx(modelName, modelPath, modelDesc, meshDesc);
			break;
		case FILE_EXTENSION_OBJ:
			successfulLoad = loadObj(modelName, modelPath, modelDesc, meshDesc);
			break;
		}

		writePGY(modelName + ".pgy", modelPath, meshDesc, (VertexType)modelDesc->vertexType_);
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
		SHOW_MESSAGEBOX(errorMsg);
	}

	return successfulLoad;
}
bool IOComponent::loadObj(
	std::string modelName, 
	std::string modelPath, 
	MdlDescModel* modelDesc, 
	MeshDesc& meshDesc)
{
	bool sucessfulMake = true;

	MeshMakerObj* objMaker = new MeshMakerObj(
		modelPath, 
		modelPath, 
		modelName, 
		modelPath,
		texNameToTexID);
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
	//	SHOW_MESSAGEBOX(errorMsg);
	//}

	delete objMaker;
	return sucessfulMake;
}
bool IOComponent::loadFbx(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc)
{
	bool successfulLoad = true;

	std::vector<LoaderFbxModelDesc> fbxModels = fbxLoader_->load(modelPath+modelName);

	std::vector<VertexDesc> vertices;
	std::vector<unsigned int> indices;
	MaterialDesc materialDesc;

	if(fbxModels.size() > 0)
	{
		fbxModels[0].getMeshDesc().createVertices(vertices, indices);
		materialDesc	= fbxModels[0].getMaterialDesc().getMaterialDesc();

		std::vector<LoaderFbxAnimationDesc> animationDescs = fbxModels[0].getAnimationDescs();

		std::map<std::string, AnimationClip*>* animations = new std::map<std::string, AnimationClip*>();
		for(unsigned int i=0; i<animationDescs.size(); i++)
		{
			animationDescs[i].convertToXKillFormat(animations);
		}

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
	else
		successfulLoad = false;

	return successfulLoad;
}
bool IOComponent::loadPGY(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc)
{
	bool sucessfulLoad = true;

	LoaderPGY pgyLoader(
		modelPath,
		modelName);
	sucessfulLoad = pgyLoader.init();

	if(sucessfulLoad)
	{
		meshDesc = pgyLoader.getMeshModel();
		//if(writeTimeUTC == pgyLoader.getWriteTimeUTC())
	}

	return sucessfulLoad;
}

bool IOComponent::writePGY(std::string modelName, std::string modelPath, MeshDesc meshDesc, VertexType vertexType)
{
	WriterPGY pgyWriter(
		meshDesc,
		//writeTimeUTC, //fix this
		modelPath,
		modelName,
		vertexType);
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

	std::string extensionFbx = "fbx";
	std::string extensionFBX = "FBX";
	std::string extensionObj = "obj";

	if(strcmp(extension.c_str(), extensionFbx.c_str()) == 0)
		type = FILE_EXTENSION_FBX;
	else if(strcmp(extension.c_str(), extensionFBX.c_str()) == 0)
		type = FILE_EXTENSION_FBX;
	else if(strcmp(extension.c_str(), extensionObj.c_str()) == 0)
		type = FILE_EXTENSION_OBJ;
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

std::vector<std::string> IOComponent::getFileNames(const LPCTSTR filename)
{
	std::vector<std::string> foundFiles;

	WIN32_FIND_DATA findFileData;
	HANDLE searchHandleWinAPI;

	searchHandleWinAPI = FindFirstFile(
		filename,
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

void IOComponent::reset()
{
}

void IOComponent::onEvent(Event* e)
{
}