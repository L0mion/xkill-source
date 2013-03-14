#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "ManagementInstance.h"
#include "MeshVerticesInstanced.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementInstance::ManagementInstance()
{
	ATTRIBUTES_INIT_ALL;
}
ManagementInstance::~ManagementInstance()
{
	//Do nothing.
}

void ManagementInstance::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Clear all buffers.
	while(itrMesh.hasNext())
	{
		unsigned int meshID = itrMesh.getNext()->meshID;
		instancedDatas_[meshID].resetStream();
	}

	//Fill instance-lists with updated data.
	while(itrRender.hasNext())
	{
		AttributePtr<Attribute_Render> renderAt = itrRender.getNext();
		if(renderAt->meshID != XKILL_Enums::ModelId::PLAYERCONTROLLEDCHARACTER)
			addInstance(renderAt);
	}

	//Update buffers with new data.
	while(itrMesh.hasNext())
	{
		unsigned int meshID = itrMesh.getNext()->meshID;
		instancedDatas_[meshID].updateDataStream(device, devcon);
	}
}

void ManagementInstance::init()
{
}

void ManagementInstance::addInstance(AttributePtr<Attribute_Render>& ptr_render)
{
	//Establish instance world matrix.
	AttributePtr<Attribute_Spatial>		ptr_spatial		= ptr_render->ptr_spatial;
	AttributePtr<Attribute_Position>	ptr_position	= ptr_spatial->ptr_position;
	
	VertexInstanced instance;
	instance.world_ = calculateWorldMatrix(ptr_spatial, ptr_position);

	//int cull = 0;
	//while(itrCamera.hasNext())
	//{
	//	unsigned int camIndex = itrCamera.getNext().index();
	//	if(ptr_render->culling.getBool(camIndex))
	//		cull = cull | 1 << camIndex;
	//}
	instance.cull_ = ptr_render->culling.values[0]; //cull;

	InstancedData& instancedData = instancedDatas_[ptr_render->meshID];
	instancedData.pushData(instance);
}

DirectX::XMFLOAT4X4 ManagementInstance::calculateWorldMatrix(
	AttributePtr<Attribute_Spatial>	ptr_spatial, 
	AttributePtr<Attribute_Position> ptr_position)
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