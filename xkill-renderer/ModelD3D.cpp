#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"

#include "ModelD3D.h"

ModelD3D::ModelD3D(
	VB* vb,
	const std::vector<IB*> ibs)
{
	vb_		= vb;
	ibs_	= ibs;
}
ModelD3D::~ModelD3D()
{
	if(vb_)
		delete vb_;
	for(unsigned int i = 0; i < ibs_.size(); i++)
	{
		if(ibs_[i])
			delete ibs_[i];
	}
}

VB* ModelD3D::getVB()
{
	return vb_;
}
const std::vector<IB*>& ModelD3D::getIBs()
{
	return ibs_;
}
