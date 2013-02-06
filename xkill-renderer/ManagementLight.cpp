#include <d3d11.h>
#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "ManagementLight.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementLight::ManagementLight()
{
	ATTRIBUTES_INIT_ALL;

	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;
	streamSpotLight_	= nullptr;
	streamPosLight_		= nullptr;
}
ManagementLight::~ManagementLight()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	SAFE_DELETE(streamSpotLight_);
	SAFE_DELETE(streamPosLight_);
}

void ManagementLight::init()
{
	streamDirLight_ = new DataStreamSRV<LightDescDir>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamPointLight_ = new DataStreamSRV<LightDescPoint>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamSpotLight_ = new DataStreamSRV<LightDescSpot>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamPosLight_ = new DataStreamSRV<Float3>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
}

void ManagementLight::reset()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	SAFE_DELETE(streamSpotLight_);
	SAFE_DELETE(streamPosLight_);

	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;
	streamSpotLight_	= nullptr;
	streamPosLight_		= nullptr;
}

void ManagementLight::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Reset counters.
	streamDirLight_		->resetStream();
	streamPointLight_	->resetStream();
	streamSpotLight_	->resetStream();
	streamPosLight_		->resetStream();

	//Send new data into streams.
	updateStreamDirLight();
	updateStreamPointLight();
	updateStreamSpotLight();
	//updateStreamPosLight();

	//Map updated streams onto buffers.
	streamDirLight_		->updateDataStream(device, devcon);
	streamPointLight_	->updateDataStream(device, devcon);
	streamSpotLight_	->updateDataStream(device, devcon);
	streamPosLight_		->updateDataStream(device, devcon);
}

void ManagementLight::setLightSRVCS(
	ID3D11DeviceContext*	devcon, 
	LightBufferType			bufferType, 
	unsigned int			shaderRegister)
{
	switch (bufferType)
	{
	case LIGHTBUFFERTYPE_DIR:
		streamDirLight_->setStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POINT:
		streamPointLight_->setStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_SPOT:
		streamSpotLight_->setStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POS_VIEW:
		streamPosLight_->setStreamSRV(devcon, shaderRegister);
		break;
	}
}
void ManagementLight::unsetLightSRVCS(
	ID3D11DeviceContext*	devcon, 
	LightBufferType			bufferType, 
	unsigned int			shaderRegister)
{
	switch (bufferType)
	{
	case LIGHTBUFFERTYPE_DIR:
		streamDirLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POINT:
		streamPointLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_SPOT:
		streamSpotLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POS_VIEW:
		streamPosLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	}
}

void ManagementLight::updateStreamDirLight()
{
	AttributePtr<Attribute_Light_Dir> ptr_lightDir;
	while(itrLightDir.hasNext())
	{
		ptr_lightDir = itrLightDir.getNext();
		streamDirLight_->pushData(ptr_lightDir->lightDir);
	}
}
void ManagementLight::updateStreamPointLight()
{
	AttributePtr<Attribute_Light_Point>	ptr_lightPoint;
	AttributePtr<Attribute_Position>	ptr_position;
	while(itrLightPoint.hasNext())
	{
		ptr_lightPoint = itrLightPoint.getNext();
		ptr_position = ptr_lightPoint->ptr_position;

		streamPointLight_->pushData(ptr_lightPoint->lightPoint);
		streamPosLight_->pushData(ptr_position->position);
	}
}
void ManagementLight::updateStreamSpotLight()
{
	AttributePtr<Attribute_Light_Spot>	ptr_lightSpot;
	AttributePtr<Attribute_Position>	ptr_position;
	while(itrLightSpot.hasNext())
	{
		ptr_lightSpot = itrLightSpot.getNext();
		ptr_position = ptr_lightSpot->ptr_position;

		streamSpotLight_->pushData(ptr_lightSpot->lightSpot);
		streamPosLight_->pushData(ptr_position->position);
	}
}

unsigned int ManagementLight::getLightDirCurCount()
{
	return streamDirLight_->getDataCountCur();
}
unsigned int ManagementLight::getLightPointCurCount()
{
	return streamPointLight_->getDataCountCur();
}
unsigned int ManagementLight::getLightSpotCurCount()
{
	return streamSpotLight_->getDataCountCur();
}

//void ManagementLight::transformLightViewSpacePoss(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 view)
//{
//	DirectX::XMFLOAT4 pos;
//	DirectX::XMVECTOR posXM;
//	DirectX::XMMATRIX viewXM;
//	for(unsigned int i = 0; i < lightPosCurCount_; i++)
//	{
//		Float3 lightPos = lightPoss_[i];
//		//Convert general-type vars into XM-type vars.
//		pos	= DirectX::XMFLOAT4(lightPos.x, lightPos.y, lightPos.z, 1.0f);
//		//posXM	= DirectX::XMLoadFloat4(&pos);
//viewXM	= DirectX::XMLoadFloat4x4(&view);
//
////Convert light position to view-space.
//posXM = DirectX::XMVector4Transform(posXM, viewXM);
//
////Writeback result to light-attribute.
//DirectX::XMStoreFloat4(&pos, posXM);
//		
//		if(i < lightPossView_.size())
//			lightPossView_[i] = LightPos(Float3(pos.x, pos.y, pos.z));
//		else
//			lightPossView_.push_back(LightPos(Float3(pos.x, pos.y, pos.z)));
//	}
//
//	if(lightPosCurCount_ > 0)
//		updateLightBuffers(devcon, LIGHTBUFFERTYPE_POS_VIEW);
//
//	/*
//	//Convert general-type vars into XM-type vars.
//	pos		= *((DirectX::XMFLOAT3*)(&(lightPoss_[i])));
//	posXM	= DirectX::XMLoadFloat3(&pos);
//	viewXM	= DirectX::XMLoadFloat4x4(&view);
//
//	//Convert light position to view-space.
//	posXM = DirectX::XMVector3Transform(posXM, viewXM);
//
//	//Writeback result to light-attribute.
//	DirectX::XMStoreFloat3(&pos, posXM);
//
//	if(i < lightPossView_.size())
//	lightPossView_[i] = *((Float3*)(&pos));
//	else
//	lightPossView_.push_back(*((Float3*)(&pos)));
//	*/
//}