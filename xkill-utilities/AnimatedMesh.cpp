#include "MeshVertices.h"

#include "AnimatedMesh.h"
#include "M3DMeshData.h"
#include "SkinnedData.h"


AnimatedMesh::AnimatedMesh()
{
	vertices_	= new std::vector<VertexPosNormTexTanSkinned>();
	indices_	= new std::vector<unsigned int>();
	subsets_	= new std::vector<M3DSubset>();
	materials_	= new std::vector<M3DMaterial>();
	skinInfo_	= new SkinnedData();

	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;

	timePosition_ = 0.0f; 
}

AnimatedMesh::~AnimatedMesh()
{
	if(vertices_)
		delete vertices_;
	if(indices_)
		delete indices_;
	if(subsets_)
		delete subsets_;
	if(materials_)
		delete materials_;
	if(skinInfo_)
		delete skinInfo_;
	if(vertexBuffer_)
		vertexBuffer_->Release();
	if(indexBuffer_)
		indexBuffer_->Release();
}

void AnimatedMesh::update(float delta)
{
	timePosition_ += delta;

	if(timePosition_ > skinInfo_->getClipEndTime("Take1"))
		timePosition_ = 0.0f;
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
	
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth			= vertices_->size() * sizeof(vertices_->at(0));
	bufferDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;

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

float AnimatedMesh::getTimePosition() const
{
	return timePosition_;
}