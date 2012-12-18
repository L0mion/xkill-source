#include "DebugShapeD3D.h"

DebugShapeD3D::DebugShapeD3D(VB* vb, IB* ib)
{
	vb_ = vb;
	ib_ = ib;
}
DebugShapeD3D::~DebugShapeD3D()
{
	if(vb_)
		delete vb_;
	if(ib_)
		delete ib_;
}

VB* DebugShapeD3D::getVB() const
{
	return vb_;
}
IB* DebugShapeD3D::getIB() const
{
	return ib_;
}