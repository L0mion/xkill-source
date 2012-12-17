#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>
#include <xkill-utilities/AttributeType.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"
#include "LoaderTexDesc.h"
#include "LoaderMdlDesc.h"
#include "MdlDesc.h"
#include "MdlDescModel.h"

IOComponent::IOComponent()
{
	texNameToTexID = nullptr;
}
IOComponent::~IOComponent()
{
	if(texNameToTexID)
		delete texNameToTexID;

	for(unsigned int i = 0; i < meshModels_.size(); i++)
	{
		if(meshModels_[i])
			delete meshModels_[i];
	}
}
bool IOComponent::init()
{
	bool sucessfulInit = true;

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

		SEND_EVENT(&Event_PostDescTex(texDesc));
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
		SHOW_MESSAGEBOX("Couldn't locate any .mdldesc-files in xkill-resources/.");

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
		MeshModel* model = objMaker->claimMesh();
		meshModels_.push_back(model);

		Event_CreateMesh e(modelDesc->modelID_, model, modelDesc->dynamic_);
		SEND_EVENT(&e);
	}
	else
	{
		std::string errorMsg = "Could not load model: " + modelPath + modelName;
		SHOW_MESSAGEBOX(errorMsg);
	}

	delete objMaker;
	return sucessfulMake;
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