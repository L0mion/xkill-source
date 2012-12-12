#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"

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

	sucessfulInit = initBth();
	if(sucessfulInit)
		sucessfulInit = initArena();
	if(sucessfulInit)
		sucessfulInit = initProjectile();

	return sucessfulInit;
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
		Event_CreateMesh e(makerProjectile_->getMesh(), true);
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