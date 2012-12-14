#include <Windows.h>
#include "IB.h"
#include "SubsetD3D.h"

SubsetD3D::SubsetD3D(
	const unsigned int	materialIndex,
	IB*					indexBuffer)
{
	materialIndex_	= materialIndex;
	indexBuffer_	= indexBuffer;
}
SubsetD3D::~SubsetD3D()
{
	if(indexBuffer_)
		delete indexBuffer_;
}

unsigned int SubsetD3D::getMaterialIndex()	const
{
	return materialIndex_;
}
IB*	SubsetD3D::getIndexBuffer()	const
{
	return indexBuffer_;
}