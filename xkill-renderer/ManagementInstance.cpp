#include <DirectXMath.h>

#include <xkill-utilities/EventManager.h>

#include "ManagementInstance.h"

ATTRIBUTES_DECLARE_ALL;

ManagementInstance::ManagementInstance()
{
	ATTRIBUTES_INIT_ALL;
}
ManagementInstance::~ManagementInstance()
{
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap_.begin(); i != instancesMap_.end(); i++)
	{
		SAFE_DELETE(i->second);
	}
}

void ManagementInstance::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Reset all buffers.
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap_.begin(); i != instancesMap_.end(); i++)
	{
		i->second->resetStream();
	}

	//Fill instance-lists with updated data.
	while(itrRender.hasNext())
	{
		Attribute_Render* renderAttribute = itrRender.getNext();
		if(renderAttribute->cull)
		{
			addRenderAtInstance(renderAttribute);
		}
	}

	//Update buffers with new data
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap_.begin(); i != instancesMap_.end(); i++)
	{
		i->second->updateDataStream(device, devcon);
	}
}

void ManagementInstance::addRenderAtInstance(Attribute_Render* renderAt)
{
	Attribute_Spatial*	spaAt = itrSpatial.at(renderAt->ptr_spatial.index);
	Attribute_Position*	posAt = itrPosition.at(spaAt->ptr_position.index);

	VertexPosNormTexInstanced newInstance;
	newInstance.world_ = calculateWorldMatrix(spaAt, posAt);

	InstancedData* instancedData = getInstancesFromMeshID(renderAt->meshID);
	if(instancedData != nullptr)
	{ //add new instance to corresponding instance vector.
		instancedData->pushData(newInstance);
	}
	else
	{ //no existing instanced data of mesh id, create new one.
		instancedData = new InstancedData(D3D11_BIND_VERTEX_BUFFER, 0);
		instancedData->pushData(newInstance);

		instancesMap_.insert(std::pair<unsigned int, InstancedData*>(renderAt->meshID, instancedData));
	}
}

InstancedData* ManagementInstance::getInstancesFromMeshID(unsigned int meshID)
{
	InstancedData* instancedData = nullptr;

	std::map<unsigned int, InstancedData*>::iterator it = instancesMap_.find(meshID);
	if(it != instancesMap_.end())
	{
		instancedData = it->second;
	}

	return instancedData;
}

DirectX::XMFLOAT4X4 ManagementInstance::calculateWorldMatrix(
	Attribute_Spatial*	spaAt, 
	Attribute_Position* posAt)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(
		posAt->position.x,
		posAt->position.y,
		posAt->position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(
		spaAt->scale.x,
		spaAt->scale.y,
		spaAt->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(
		spaAt->rotation.x,
		spaAt->rotation.y,
		spaAt->rotation.z,
		spaAt->rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, mWorldMatrix);

	return worldMatrix;
}

std::map<
	unsigned int,
	InstancedData*>& ManagementInstance::getInstancesMap()
{
	return instancesMap_;
}