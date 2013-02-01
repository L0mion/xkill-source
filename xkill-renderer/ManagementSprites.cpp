
#include "ManagementSprites.h"
#include "renderingUtilities.h"

ManagementSprites::ManagementSprites()
{
	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;
}
ManagementSprites::~ManagementSprites()
{
	SAFE_RELEASE(vertexBuffer_);
	SAFE_RELEASE(indexBuffer_);
}

void ManagementSprites::reset()
{
	SAFE_RELEASE(vertexBuffer_);
	SAFE_RELEASE(indexBuffer_);
}

HRESULT ManagementSprites::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	initVertices();
	initIndices();

	hr = initVertexBuffer(device);
	if(SUCCEEDED(hr))
		hr = initIndexBuffer(device);

	return hr;
}

void ManagementSprites::initVertices()
{
	vertices_[0] = VertexPosNormTex(Float3(-0.5f, 0.5f, 0.0f), Float3(0.0f, 0.0f, 0.0f), Float2(0.0f, 0.0f));
	vertices_[1] = VertexPosNormTex(Float3(0.5f, 0.5f, 0.0f), Float3(0.0f, 0.0f, 0.0f), Float2(1.0f, 0.0f));
	vertices_[2] = VertexPosNormTex(Float3(-0.5f, -0.5f, 0.0f), Float3(0.0f, 0.0f, 0.0f), Float2(0.0f, 1.0f));
	vertices_[3] = VertexPosNormTex(Float3(0.5f, -0.5f, 0.0f), Float3(0.0f, 0.0f, 0.0f), Float2(1.0f, 1.0f));
}
void ManagementSprites::initIndices()
{
	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;
	indices_[3] = 3;
	indices_[4] = 2;
	indices_[5] = 1;
}
HRESULT ManagementSprites::initVertexBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage		  = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth	  = sizeof(VertexPosNormTex) * NUM_SPRITE_VERTICES;
	bufferDesc.BindFlags	  = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices_[0];

	hr = device->CreateBuffer(&bufferDesc, &vinitData, &vertexBuffer_);

	return hr;
}
HRESULT ManagementSprites::initIndexBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(DWORD) * NUM_SPRITE_INDICES;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices_[0];

	hr = device->CreateBuffer(&bufferDesc, &iinitData, &indexBuffer_);

	return hr;
}

ID3D11Buffer* ManagementSprites::getVertexBuffer()
{
	return vertexBuffer_;
}
ID3D11Buffer* ManagementSprites::getIndexBuffer()
{
	return indexBuffer_;
}