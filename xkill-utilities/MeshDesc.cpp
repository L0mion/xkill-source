#include "MeshDesc.h"

MeshDesc::MeshDesc()
{
	skinnedData_ = nullptr;
}
MeshDesc::~MeshDesc()
{
	if(skinnedData_)
		delete skinnedData_;
}

bool MeshDesc::hasAnimation()
{
	bool animation = false;
	if(skinnedData_)
	{
		if(skinnedData_->getAnimations()->size() > 0)
			animation = true;
	}
	return animation;
}