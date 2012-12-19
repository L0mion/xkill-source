#include "VB.h"
#include "IB.h"
#include "DebugShapeD3D.h"

DebugShapeD3D::DebugShapeD3D(VB<VertexPosColor>* vb)
{
	vb_ = vb;
}
DebugShapeD3D::~DebugShapeD3D()
{
	if(vb_)
		delete vb_;
}

VB<VertexPosColor>* DebugShapeD3D::getVB() const
{
	return vb_;
}