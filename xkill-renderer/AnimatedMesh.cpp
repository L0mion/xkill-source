#include <xkill-utilities/MeshVertices.h>

#include "AnimatedMesh.h"
#include "M3DMeshData.h"
#include "SkinnedData.h"

#include "renderingUtilities.h"

AnimatedMesh::AnimatedMesh()
{
	vertices_	= new std::vector<VertexPosNormTexTanSkinned>();
	indices_	= new std::vector<unsigned int>();
	subsets_	= new std::vector<M3DSubset>();
	materials_	= new std::vector<M3DMaterial>();
	skinInfo_	= new SkinnedData();

	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;
}

AnimatedMesh::~AnimatedMesh()
{
	SAFE_DELETE(vertices_);
	SAFE_DELETE(indices_);
	SAFE_DELETE(subsets_);
	SAFE_DELETE(materials_);
	SAFE_DELETE(skinInfo_);

	SAFE_RELEASE(vertexBuffer_);
	SAFE_RELEASE(indexBuffer_);
}

HRESULT AnimatedMesh::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = createVertexBuffer(device);
	if(SUCCEEDED(hr))
		hr = createIndexBuffer(device);

	return hr;
}

HRESULT AnimatedMesh::createVertexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth		= vertices_->size() * sizeof(VertexPosNormTexTanSkinned);
	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &vertices_->at(0);

	HRESULT hr = device->CreateBuffer(&bufferDesc, &vInitData, &vertexBuffer_);

	return hr;
}
HRESULT AnimatedMesh::createIndexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth		= sizeof(unsigned int) * indices_->size();
	bufferDesc.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = &indices_->at(0);

	HRESULT hr = device->CreateBuffer(&bufferDesc, &iInitData, &indexBuffer_);

	return hr;
}

std::vector<VertexPosNormTexTanSkinned>* AnimatedMesh::getVertices()	const
{
	return vertices_;
}
std::vector<unsigned int>*				 AnimatedMesh::getIndices()		const
{
	return indices_;
}
std::vector<M3DSubset>*					 AnimatedMesh::getSubsets()		const
{
	return subsets_;
}
std::vector<M3DMaterial>*				 AnimatedMesh::getMaterials()	const
{
	return materials_;
}
SkinnedData*							 AnimatedMesh::getSkinInfo()	const
{
	return skinInfo_;
}

ID3D11Buffer* AnimatedMesh::getVertexBuffer()	const
{
	return vertexBuffer_;
}
ID3D11Buffer* AnimatedMesh::getIndexBuffer()	const			
{
	return indexBuffer_;
}
unsigned int  AnimatedMesh::getNumVertices()	const
{
	return vertices_->size();
}
unsigned int  AnimatedMesh::getNumIndices()		const
{
	return indices_->size();
}