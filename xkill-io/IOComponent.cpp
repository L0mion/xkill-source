#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "IOComponent.h"

IOComponent::IOComponent()
{
	makerObj_ = nullptr;
}
IOComponent::~IOComponent()
{
	if(makerObj_)
		delete makerObj_;
}
bool IOComponent::init()
{
	makerObj_ = new MeshMakerObj(
		bthPath,
		pgyPath,
		bthName,
		bthMTLPath);
	bool sucessfulMake = makerObj_->init();

	if(sucessfulMake)
	{
		Event_createMesh e(makerObj_->getMesh());
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