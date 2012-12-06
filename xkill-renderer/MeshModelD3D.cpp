#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"

#include "MeshModelD3D.h"

MeshModelD3D::MeshModelD3D(
	VB* vb,
	const std::vector<IB*> ibs)
{
	vb_		= vb;
	ibs_	= ibs;
}
MeshModelD3D::~MeshModelD3D()
{
	if(vb_)
		delete vb_;
	for(unsigned int i = 0; i < ibs_.size(); i++)
	{
		if(ibs_[i])
			delete ibs_[i];
	}
}

VB* MeshModelD3D::getVB()
{
	return vb_;
}
const std::vector<IB*>& MeshModelD3D::getIBs()
{
	return ibs_;
}
