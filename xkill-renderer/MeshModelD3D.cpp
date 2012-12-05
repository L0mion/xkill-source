#include <d3d11.h>

#include "renderingUtilities.h"

#include "MeshModelD3D.h"

MeshModelD3D::MeshModelD3D(
	ID3D11Buffer*						vertexBuffer,
	const std::vector<ID3D11Buffer*>	indexBuffers)
{
	vertexBuffer_ = vertexBuffer;
	indexBuffers_ = indexBuffers;
}
MeshModelD3D::~MeshModelD3D()
{
	SAFE_RELEASE(vertexBuffer_);
	
	for(unsigned int i = 0; i < indexBuffers_.size(); i++)
		SAFE_RELEASE(indexBuffers_[i]);
}

const std::vector<ID3D11Buffer*>&	MeshModelD3D::getIndexBuffers()
{
	return indexBuffers_;
}
ID3D11Buffer*					MeshModelD3D::getVertexBuffer()
{
	return vertexBuffer_;
}