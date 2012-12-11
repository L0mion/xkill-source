#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"
#include "LoaderTexDesc.h"

IOComponent::IOComponent()
{
	makerBTH_ = nullptr;
	makerArena_ = nullptr;
	makerProjectile_ = nullptr;
}
IOComponent::~IOComponent()
{
	if(makerBTH_)
		delete makerBTH_;
	if(makerArena_)
		delete makerArena_;
	if(makerProjectile_)
		delete makerProjectile_;
}
bool IOComponent::init()
{
	bool sucessfulInit = true;

	initTexDescs();

	sucessfulInit = initBth();
	if(sucessfulInit)
		sucessfulInit = initArena();
	if(sucessfulInit)
		sucessfulInit = initProjectile();

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

		Event_PostDescTex e(texDesc);
		SEND_EVENT(&e);
	}
	
	//Clear memory allocated
	delete loader;

	return sucessfulLoad;
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

bool IOComponent::initBth()
{
	bool sucessfulMake = true;

	makerBTH_ = new MeshMakerObj(
		objPath,
		pgyPath,
		bthName,
		mtlPath);
	sucessfulMake = makerBTH_->init();

	MeshModel* model = makerBTH_->getMesh();

	if(sucessfulMake)
	{
		Event_CreateMesh e(makerBTH_->getMesh(), true);
		SEND_EVENT(&e);
	}

	return sucessfulMake;
}
bool IOComponent::initArena()
{
	bool sucessfulMake = true;

	makerArena_ = new MeshMakerObj(
		objPath,
		pgyPath,
		arenaName,
		mtlPath);
	sucessfulMake = makerArena_->init();

	if(sucessfulMake)
	{
		Event_CreateMesh e(makerArena_->getMesh(), false);
		SEND_EVENT(&e);
	}

	return sucessfulMake;
}
bool IOComponent::initProjectile()
{
	bool sucessfulMake = true;

	makerProjectile_ = new MeshMakerObj(
		objPath,
		pgyPath,
		projectileName,
		mtlPath);
	sucessfulMake = makerProjectile_->init();

	if(sucessfulMake)
	{
		Event_CreateMesh e(makerProjectile_->getMesh(), false);
		SEND_EVENT(&e);
	}

	return sucessfulMake;
}

void IOComponent::reset()
{
}

void IOComponent::onEvent(Event* e)
{
}