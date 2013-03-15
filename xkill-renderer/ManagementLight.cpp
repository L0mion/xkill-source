#include <d3d11.h>
#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "ManagementLight.h"
#include "renderingUtilities.h"
#include "CameraLights.h"

ATTRIBUTES_DECLARE_ALL;

ManagementLight::ManagementLight()
{
	ATTRIBUTES_INIT_ALL;

	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;
}
ManagementLight::~ManagementLight()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	
	for(
		auto i = cameraLightsMap_.begin();
		i != cameraLightsMap_.end();
		i++)
	{
		SAFE_DELETE(i->second);
	}
}

void ManagementLight::init()
{
	streamDirLight_ = new DataStreamSRV<LightDescDir>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamPointLight_ = new DataStreamSRV<LightDescPoint>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
}

void ManagementLight::reset()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	
	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;

	for(
		auto i = cameraLightsMap_.begin();
		i != cameraLightsMap_.end();
	i++)
	{
		i->second->reset();
	}
}

void ManagementLight::update(ID3D11Device* device, ID3D11DeviceContext* devcon, std::vector<ViewportData> vpDatas)
{
	//Reset counters.
	streamDirLight_		->resetStream();
	streamPointLight_	->resetStream();
	for(
		auto i = cameraLightsMap_.begin();
		i != cameraLightsMap_.end();
		i++)
	{
		i->second->resetLights();
	}

	//Send new data into streams.
	updateStreamDirLight(vpDatas);
	updateStreamPointLight(vpDatas);

	//Map updated streams onto buffers.
	streamDirLight_		->updateDataStream(device, devcon);
	streamPointLight_	->updateDataStream(device, devcon);
	for(
		auto i = cameraLightsMap_.begin();
		i != cameraLightsMap_.end();
		i++)
	{
		i->second->updateLights(device, devcon);
	}
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
	}
}
void ManagementLight::setPosDirSRVCS(
	ID3D11DeviceContext* devcon,
	unsigned int camIndex,
	unsigned int shaderRegisterPos,
	unsigned int shaderRegisterDir)
{
	CameraLights* cameraLights = getCameraLightsFromCameraIndex(camIndex);
	if(cameraLights != nullptr)
		cameraLights->setSRV(devcon, shaderRegisterPos, shaderRegisterDir);
	else
		throw 0; //etmp
}
void ManagementLight::unsetPosDirSRVCS(
	ID3D11DeviceContext* devcon,
	unsigned int camIndex,
	unsigned int shaderRegisterPos,
	unsigned int shaderRegisterDir)
{
	CameraLights* cameraLights = getCameraLightsFromCameraIndex(camIndex);
	if(cameraLights != nullptr)
		cameraLights->unsetSRV(devcon, shaderRegisterPos, shaderRegisterDir);
	else
		throw 0; //etmp
}

void ManagementLight::updateStreamDirLight(std::vector<ViewportData> vpDatas)
{
	AttributePtr<Attribute_Light_Dir> ptr_lightDir;
	while(itrLightDir.hasNext())
	{
		ptr_lightDir = itrLightDir.getNext();
		streamDirLight_->pushData(ptr_lightDir->lightDir);

		Float3 dir = ptr_lightDir->direction;
		for(unsigned int i = 0; i < vpDatas.size(); i++)
		{
			addDirLightDir(
				vpDatas[i].camIndex, 
				DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(dir.x, dir.y, dir.z, 0.0f)), 
				vpDatas[i].view);
		}
	}
}
void ManagementLight::updateStreamPointLight(std::vector<ViewportData> vpDatas)
{
	AttributePtr<Attribute_Light_Point>	ptr_lightPoint;
	AttributePtr<Attribute_Position>	ptr_position;
	while(itrLightPoint.hasNext())
	{
		ptr_lightPoint = itrLightPoint.getNext();
		ptr_position = ptr_lightPoint->ptr_position;
		streamPointLight_->pushData(ptr_lightPoint->lightPoint);

		Float3 pos = ptr_position->position;
		for(unsigned int i = 0; i < vpDatas.size(); i++)
		{
			addPointLightPos(
				vpDatas[i].camIndex,
				DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(pos.x, pos.y, pos.z, 1.0f)),
				vpDatas[i].view);
		}
	}
}

unsigned int ManagementLight::getNumPos(unsigned int camIndex)
{
	unsigned int numLights = 0;

	CameraLights* cameraLights = nullptr;
	cameraLights = getCameraLightsFromCameraIndex(camIndex);
	if(cameraLights != nullptr)
	{
		numLights = cameraLights->getNumPos();
	}

	return numLights;
}

void ManagementLight::addDirLightDir(unsigned int camIndex, DirectX::XMVECTOR dir, DirectX::XMFLOAT4X4 view)
{
	//Transform to view-space
	DirectX::XMVECTOR b = DirectX::XMVector4Transform(dir, DirectX::XMLoadFloat4x4(&view));
	DirectX::XMFLOAT4 c; DirectX::XMStoreFloat4(&c, b);

	CameraLights* cameraLights = getCameraLightsFromCameraIndex(camIndex);
	if(cameraLights != nullptr)
	{
		cameraLights->addDirDirLight(Float3(c.x, c.y, c.z));
	}
	else
	{
		//Add new map
		cameraLights = new CameraLights();
		cameraLights->init();
		cameraLightsMap_.insert(
			std::pair<unsigned int, CameraLights*>(
				camIndex,
				cameraLights));

		cameraLights->addDirDirLight(Float3(c.x, c.y, c.z));
	}
}
void ManagementLight::addPointLightPos(unsigned int camIndex, DirectX::XMVECTOR pos, DirectX::XMFLOAT4X4 view)
{
	//Transform to view-space
	DirectX::XMVECTOR b = DirectX::XMVector4Transform(pos, DirectX::XMLoadFloat4x4(&view));
	DirectX::XMFLOAT4 c; DirectX::XMStoreFloat4(&c, b);

	CameraLights* cameraLights = getCameraLightsFromCameraIndex(camIndex);
	if(cameraLights != nullptr)
	{
		cameraLights->addPosPointLight(Float3(c.x, c.y, c.z));
	}
	else
	{
		//Add new map
		cameraLights = new CameraLights();
		cameraLights->init();
		cameraLightsMap_.insert(
			std::pair<unsigned int, CameraLights*>(
			camIndex,
			cameraLights));

		cameraLights->addPosPointLight(Float3(c.x, c.y, c.z));
	}
}

CameraLights* ManagementLight::getCameraLightsFromCameraIndex(unsigned int camIndex)
{
	CameraLights* cameraLights = nullptr;

	auto it = cameraLightsMap_.find(camIndex);
	if(it != cameraLightsMap_.end())
	{
		cameraLights = it->second;
	}

	return cameraLights;
}

unsigned int ManagementLight::getLightDirCurCount()
{
	return streamDirLight_->getDataCountCur();
}
unsigned int ManagementLight::getLightPointCurCount()
{
	return streamPointLight_->getDataCountCur();
}