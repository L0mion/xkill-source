#include "CameraLights.h"

CameraLights::CameraLights()
{
	streamPosPointLight_	= nullptr;
	streamDirDirLight_		= nullptr;
}
CameraLights::~CameraLights()
{
	SAFE_DELETE(streamPosPointLight_);
	SAFE_DELETE(streamDirDirLight_);
}

void CameraLights::init()
{
	streamPosPointLight_ = new DataStreamSRV<Float3>(
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamDirDirLight_ = new DataStreamSRV<Float3>(
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
}
void CameraLights::reset()
{
	SAFE_DELETE(streamPosPointLight_);
	SAFE_DELETE(streamDirDirLight_);

	streamPosPointLight_	= nullptr;
	streamDirDirLight_		= nullptr;
}

void CameraLights::resetLights()
{
	streamPosPointLight_->resetStream();
	streamDirDirLight_	->resetStream();
}
void CameraLights::updateLights(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	streamPosPointLight_->updateDataStream(device, devcon);
	streamDirDirLight_	->updateDataStream(device, devcon);
}

void CameraLights::addPosPointLight(Float3 pointLightPos)
{
	streamPosPointLight_->pushData(pointLightPos);
}
void CameraLights::addDirDirLight(Float3 dirLightDir)
{
	streamDirDirLight_->pushData(dirLightDir);
}

void CameraLights::setSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegisterPos, unsigned int shaderRegisterDir)
{
	streamPosPointLight_->setStreamSRV(devcon, shaderRegisterPos);
	streamDirDirLight_->setStreamSRV(devcon, shaderRegisterDir);
}
void CameraLights::unsetSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegisterPos, unsigned int shaderRegisterDir)
{
	streamPosPointLight_->unsetStreamSRV(devcon, shaderRegisterPos);
	streamDirDirLight_->unsetStreamSRV(devcon, shaderRegisterDir);
}