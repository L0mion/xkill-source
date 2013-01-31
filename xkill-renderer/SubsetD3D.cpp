#include <d3d11.h>

#include "IB.h"
#include "SubsetD3D.h"
#include "renderingUtilities.h"

SubsetD3D::SubsetD3D(
	const unsigned int	materialIndex,
	const unsigned int	numIndices,
	ID3D11Buffer*		indexBuffer)
{
	materialIndex_	= materialIndex;
	numIndices_		= numIndices;
	indexBuffer_	= indexBuffer;
}
SubsetD3D::~SubsetD3D()
{
	SAFE_RELEASE(indexBuffer_);
}

unsigned int SubsetD3D::getMaterialIndex()	const
{
	return materialIndex_;
}
unsigned int SubsetD3D::getNumIndices()		const
{
	return numIndices_;
}
ID3D11Buffer* SubsetD3D::getIndexBuffer()	const
{
	return indexBuffer_;
}