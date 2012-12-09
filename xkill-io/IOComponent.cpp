#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"

IOComponent::IOComponent()
{
	makerBTH_ = nullptr;
	makerArena_ = nullptr;
}
IOComponent::~IOComponent()
{
	if(makerBTH_)
		delete makerBTH_;
	if(makerArena_)
		delete makerArena_;
}
bool IOComponent::init()
{
	bool sucessfulInit = true;

	sucessfulInit = initBth();
	if(sucessfulInit)
		sucessfulInit = initArena();

	return sucessfulInit;
}
bool IOComponent::initBth()
{
	bool sucessfulMake = true;

	makerBTH_ = new MeshMakerObj(
		"../../xkill-resources/xkill-models/", //objPath,
		"../../xkill-resources/xkill-models/", //pgyPath,
		"ChessKing.obj",						//bthName,
		"../../xkill-resources/xkill-models/"); //mtlPath);
	sucessfulMake = makerBTH_->init();

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

void IOComponent::reset()
{
}

void IOComponent::onEvent(Event* e)
{
}