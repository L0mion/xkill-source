#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "ManagementInstance.h"

ATTRIBUTES_DECLARE_ALL;

ManagementInstance::ManagementInstance()
{
	ATTRIBUTES_INIT_ALL;
}
ManagementInstance::~ManagementInstance()
{
	for(unsigned int i = 0; i < meshInstances_.size(); i++)
	{
		for(unsigned int j = 0; j < meshInstances_[i].size(); j++)
		{
			SAFE_DELETE(meshInstances_[i][j]);
		}
	}
	for(unsigned int i = 0; i < shadowInstances_.size(); i++)
	{
		SAFE_DELETE(shadowInstances_[i]);
	}
}

void ManagementInstance::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Gather meshids
	std::vector<unsigned int> meshIDs;
	while(itrMesh.hasNext())
	{
		meshIDs.push_back(itrMesh.getNext()->meshID);
	}

	//Clear all buffers.
	for(unsigned int i = 0; i < meshIDs.size(); i++)
	{
		while(itrCamera.hasNext())
		{
			unsigned int camIndex = itrCamera.getNext().index();

			if(meshInstances_[meshIDs[i]][camIndex] != nullptr)
				meshInstances_[meshIDs[i]][camIndex]->resetStream();
			else
				meshInstances_[meshIDs[i]][camIndex] = new InstancedData(D3D11_BIND_VERTEX_BUFFER, 0);
		}
	}

	for(unsigned int i = 0; i < meshIDs.size(); i++)
	{
		if(shadowInstances_[meshIDs[i]] != nullptr)
			shadowInstances_[meshIDs[i]]->resetStream();
		else
			shadowInstances_[meshIDs[i]] = new InstancedData(D3D11_BIND_VERTEX_BUFFER, 0);
	}

	//Fill instance-lists with updated data.
	while(itrRender.hasNext())
	{
		AttributePtr<Attribute_Render> renderAt = itrRender.getNext();
		if(renderAt->meshID != XKILL_Enums::ModelId::PLAYERCONTROLLEDCHARACTER)
			addInstance(renderAt) ;
	}

	//Update buffers with new data.
	for(unsigned int i = 0; i < meshIDs.size(); i++)
	{
		while(itrCamera.hasNext())
		{
			unsigned int camIndex = itrCamera.getNext().index();
			meshInstances_[meshIDs[i]][camIndex]->updateDataStream(device, devcon);
		}
	}
	
	for(unsigned int i = 0; i < meshIDs.size(); i++)
		shadowInstances_[meshIDs[i]]->updateDataStream(device, devcon);
}

void ManagementInstance::init()
{
	meshInstances_.resize(MAX_MESH_ID);
	for(unsigned int i = 0; i < meshInstances_.size(); i++)
	{
		meshInstances_[i].resize(MAX_CAMERAS);
		for(unsigned int j = 0; j < meshInstances_[i].size(); j++)
			meshInstances_[i][j] = nullptr;
	}
	shadowInstances_.resize(MAX_MESH_ID);
	for(unsigned int i = 0; i < shadowInstances_.size(); i++)
		shadowInstances_[i] = nullptr;
}

InstancedData* ManagementInstance::getInstancedData(unsigned int camIndex, unsigned int meshID)
{
	return meshInstances_[meshID][camIndex];
}
InstancedData* ManagementInstance::getShadowData(unsigned int meshID)
{
	return shadowInstances_[meshID];
}

void ManagementInstance::addInstance(AttributePtr<Attribute_Render>& ptr_render)
{
	//Establish instance world matrix.
	unsigned int meshID = ptr_render->meshID;
	AttributePtr<Attribute_Spatial>		ptr_spatial		= ptr_render->ptr_spatial;
	AttributePtr<Attribute_Position>	ptr_position	= ptr_spatial->ptr_position;
	
	VertexInstanced instance;
	instance.world_ = calculateWorldMatrix(ptr_spatial, ptr_position);

	//Add instance to each valid camera-object.
	while(itrCamera.hasNext())
	{
		unsigned int camIndex = itrCamera.getNext().index();
		if(ptr_render->culling.getBool(camIndex))
		{
			meshInstances_[meshID][camIndex]->pushData(instance);
		}
	}

	if(meshID == 200)
		shadowInstances_[meshID]->pushData(instance);
}

DirectX::XMFLOAT4X4 ManagementInstance::calculateWorldMatrix(
	AttributePtr<Attribute_Spatial>&	ptr_spatial, 
	AttributePtr<Attribute_Position>& ptr_position)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(
		ptr_position->position.x,
		ptr_position->position.y,
		ptr_position->position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(
		ptr_spatial->scale.x,
		ptr_spatial->scale.y,
		ptr_spatial->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(
		ptr_spatial->rotation.x,
		ptr_spatial->rotation.y,
		ptr_spatial->rotation.z,
		ptr_spatial->rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, mWorldMatrix);

	return worldMatrix;
}