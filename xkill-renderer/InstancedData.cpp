#include <d3d11.h>

#include <xkill-utilities/EventManager.h>

#include "MeshVerticesInstanced.h"
#include "InstancedData.h"
#include "renderingUtilities.h"

InstancedData::InstancedData()
{
	instanceCount_	= 0;
	instanceMax_	= INSTANCED_DATA_START_MAX_INSTANCES;

	instanceBuffer_ = nullptr;

	instanceVec_ = std::vector<VertexPosNormTexInstanced>(instanceMax_);
}
InstancedData::~InstancedData()
{
	SAFE_RELEASE(instanceBuffer_);
}

void InstancedData::addInstance(VertexPosNormTexInstanced instance)
{
	if(instanceCount_ < instanceVec_.size())
	{
		instanceVec_[instanceCount_] = instance;
	}
	else
	{
		instanceVec_.push_back(instance);
	}

	instanceCount_++;
}

void InstancedData::reset()
{
	instanceCount_ = 0;
	instanceVec_.clear();
}

HRESULT InstancedData::initBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(instanceCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth			= sizeof(VertexPosNormTexInstanced) * instanceMax_;
		bufferDesc.MiscFlags			= 0;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &instanceVec_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &instanceBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"InstancedData::init device->CreateBuffer failed!");
	}

	return hr;
}

HRESULT InstancedData::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	HRESULT hr = S_OK;

	if(instanceCount_ > 0)
	{
		if(!instanceBuffer_)
			hr = increaseBufferCapacity(device, instanceCount_);

		if(instanceCount_ > instanceMax_)
		{ //resize buffer
			hr = increaseBufferCapacity(device, instanceCount_);
		}
		hr = updateBuffer(devcon);
	}

	return hr;
}

HRESULT InstancedData::increaseBufferCapacity(ID3D11Device* device, unsigned int target)
{
	HRESULT hr = S_OK;

	incrementMax(target);
	SAFE_RELEASE(instanceBuffer_);
	hr = initBuffer(device);

	return hr;
}
HRESULT InstancedData::updateBuffer(ID3D11DeviceContext* devcon)
{
	HRESULT hr = S_OK;

	D3D11_MAPPED_SUBRESOURCE map;
	hr = devcon->Map(
		instanceBuffer_, 
		NULL, 
		D3D11_MAP_WRITE_DISCARD, 
		NULL, 
		&map);
	if(SUCCEEDED(hr))
	{
		memcpy(map.pData, &instanceVec_[0], instanceCount_ * sizeof(VertexPosNormTexInstanced));
		devcon->Unmap(instanceBuffer_, NULL);
	}
	else
	{
		ERROR_MSG(L"InstancedData::updateBuffer devcon->Map failed!");
	}

	return hr;
}
void InstancedData::incrementMax(unsigned int target)
{
	if(instanceMax_ > 0)
	{
		while(instanceMax_ < target)
		{
			instanceMax_ *= 2;
		}
	}
}

unsigned int InstancedData::getInstanceCount() const
{
	return instanceCount_;
}
std::vector<VertexPosNormTexInstanced>& InstancedData::getInstanceVec()
{
	return instanceVec_;
}
ID3D11Buffer* InstancedData::getInstanceBuffer() const
{
	return instanceBuffer_;
}