#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshModel.h>

#include "MeshMakerObj.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent()
{
	makerObj_ = nullptr;
}
MeshComponent::~MeshComponent()
{
	if(makerObj_)
		delete makerObj_;
}
bool MeshComponent::init()
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
void MeshComponent::reset()
{
}