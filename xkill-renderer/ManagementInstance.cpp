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
		delete i->second;
	}
}

void ManagementInstance::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Clear all existing instance-lists.
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap_.begin(); i != instancesMap_.end(); i++)
	{
		i->second->reset();
	}

	//Fill instance-lists with updated data.
	while(itrRender.hasNext())
	{
		addRenderAtInstance(itrRender.getNext());
	}

	//Update each instance-list. (fill buffers with data)
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap_.begin(); i != instancesMap_.end(); i++)
	{
		i->second->update(device, devcon);
	}
}

void ManagementInstance::addRenderAtInstance(Attribute_Render* renderAt)
{
	Attribute_Spatial*	spaAt = itrSpatial.at(renderAt->ptr_spatial.index);

	VertexPosNormTexInstanced newInstance;
	newInstance.world_ = calculateWorldMatrix(spaAt);

	InstancedData* instancedData = getInstancesFromMeshID(renderAt->meshID);
	if(instancedData != nullptr)
	{ //add new instance to corresponding instance vector.
		instancedData->addInstance(newInstance);
	}
	else
	{ //no existing instanced data of mesh id, create new one.
		instancedData = new InstancedData();
		instancedData->addInstance(newInstance);

		instancesMap_.insert(std::pair<unsigned int, InstancedData*>(renderAt->meshID, instancedData));
		//instancesMap_[renderAt->meshID] = instancedData;
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
	Attribute_Spatial*	spaAt)
{
	Float3 position = spaAt->position();
	Float4 rotation = spaAt->rotation();

	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(
		position.x,
		position.y,
		position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(
		spaAt->scale.x,
		spaAt->scale.y,
		spaAt->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(
		rotation.x,
		rotation.y,
		rotation.z,
		rotation.w);

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