#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"

IOComponent::IOComponent()
{
	makerBTH_ = nullptr;
	makerArena1_ = nullptr;
	makerArena2_ = nullptr;
	makerProjectile_ = nullptr;
}
IOComponent::~IOComponent()
{
	if(makerBTH_)
		delete makerBTH_;
	if(makerArena1_)
		delete makerArena1_;
	if(makerArena2_)
		delete makerArena2_;
	if(makerProjectile_)
		delete makerProjectile_;
}
bool IOComponent::init()
{
	bool sucessfulInit = true;

	sucessfulInit = initBth();
	if(sucessfulInit)
		sucessfulInit = initArena1();
	if(sucessfulInit)
		sucessfulInit = initArena2();
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

bool IOComponent::initArena1()
{
	bool sucessfulMake = true;

	makerArena1_ = new MeshMakerObj(
		objPath,
		pgyPath,
		arenaName1,
		mtlPath);
	sucessfulMake = makerArena1_->init();

	if(sucessfulMake)
	{
		Event_CreateMesh e(makerArena1_->getMesh(), false);
		SEND_EVENT(&e);
	}

	return sucessfulMake;
}

bool IOComponent::initArena2()
{
	bool sucessfulMake = true;

	makerArena2_ = new MeshMakerObj(
		objPath,
		pgyPath,
		arenaName2,
		mtlPath);
	sucessfulMake = makerArena2_->init();

	if(sucessfulMake)
	{
		Event_CreateMesh e(makerArena2_->getMesh(), false);
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