#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "CameraInstances.h"
#include "ManagementInstance.h"

ATTRIBUTES_DECLARE_ALL;

ManagementInstance::ManagementInstance()
{
	ATTRIBUTES_INIT_ALL;

	shadowInstances_ = nullptr;
}
ManagementInstance::~ManagementInstance()
{
	for(
		auto i = cameraInstancesMap_.begin(); 
		i != cameraInstancesMap_.end(); 
		i++)
	{
		SAFE_DELETE(i->second);
	}

	SAFE_DELETE(shadowInstances_);
}

void ManagementInstance::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Clear all buffers.
	for(
		auto i = cameraInstancesMap_.begin(); 
		i != cameraInstancesMap_.end(); 
		i++)
	{
		i->second->reset();
	}
	shadowInstances_->reset();

	//Fill instance-lists with updated data.
	while(itrRender.hasNext())
	{
		AttributePtr<Attribute_Render> renderAt = itrRender.getNext();
		if(renderAt->meshID != XKILL_Enums::ModelId::PLAYERCONTROLLEDCHARACTER)
			addInstance(renderAt);
	}

	//Update buffers with new data.
	for(
		auto i = cameraInstancesMap_.begin(); 
		i != cameraInstancesMap_.end(); 
		i++)
	{
		i->second->update(device, devcon);
	}
	shadowInstances_->update(device, devcon);
}

void ManagementInstance::init()
{
	shadowInstances_ = new CameraInstances();
}

void ManagementInstance::addInstance(AttributePtr<Attribute_Render> ptr_render)
{
	//Establish instance world matrix.
	AttributePtr<Attribute_Spatial>		ptr_spatial		= ptr_render->ptr_spatial;
	AttributePtr<Attribute_Position>	ptr_position	= ptr_spatial->ptr_position;
	
	VertexInstanced instance;
	instance.world_ = calculateWorldMatrix(ptr_spatial, ptr_position);

	//Add instance to each valid camera-object.
	while(itrCamera.hasNext())
	{
		AttributePtr<Attribute_Camera> ptr_camera = itrCamera.getNext();
		if(ptr_render->culling.getBool(ptr_camera.index()))
		{
			addCameraInstance(ptr_camera, ptr_render->meshID, instance);
		}
	}

	if(ptr_render->meshID == 200)
		shadowInstances_->addInstance(ptr_render->meshID, instance);
}
void ManagementInstance::addCameraInstance(
	AttributePtr<Attribute_Camera> ptr_camera,
	unsigned int meshID,
	VertexInstanced instance)
{
	CameraInstances* camInstances = getCameraInstancesFromCameraIndex(ptr_camera.index());
	if(camInstances != nullptr)
	{ //Add new instance to corresponding CameraInstances-object.
		camInstances->addInstance(meshID, instance);
	}
	else
	{ //No existing CameraInstances-object. Make a new one.
		camInstances = new CameraInstances();
		camInstances->addInstance(meshID, instance);
		cameraInstancesMap_.insert(
			std::pair<unsigned int, CameraInstances*>(
			ptr_camera.index(), 
			camInstances));
	}
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

CameraInstances* ManagementInstance::getCameraInstancesFromCameraIndex(unsigned int camIndex)
{
	CameraInstances* cameraInstances = nullptr;

	auto it = cameraInstancesMap_.find(camIndex);
	if(it != cameraInstancesMap_.end())
	{
		cameraInstances = it->second;
	}

	return cameraInstances;
}
CameraInstances* ManagementInstance::getShadowInstances()
{
	return shadowInstances_;
}