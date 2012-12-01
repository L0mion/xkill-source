#include "renderingUtilities.h"
#include "MeshMakerObj.h"
#include "MeshModel.h"

#include <MeshManagement.h>

MeshManagement::MeshManagement()
{
	makerObj_ = nullptr;
}
MeshManagement::~MeshManagement()
{
	if(makerObj_)
		delete makerObj_;

	for(unsigned int i = 0; i < models_.size(); i++)
		SAFE_DELETE(models_[i]);
}

bool MeshManagement::init()
{
	makerObj_ = new MeshMakerObj(
		bthPath,
		pgyPath,
		bthName,
		bthMTLPath);
	bool sucessfulMake = makerObj_->init();

	if(sucessfulMake)
		models_.push_back(makerObj_->getMesh());

	return sucessfulMake;
}
void MeshManagement::reset()
{

}