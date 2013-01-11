#include <d3d11.h>

#include <xkill-utilities/EventManager.h>

#include "ManagementLight.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementLight::ManagementLight()
{
	lightDirMaxCount_ = LIGHT_START_MAX_COUNT_DIR;
	lightDirCurCount_ = 0;

	lightPointMaxCount_ = LIGHT_START_MAX_COUNT_POINT;
	lightPointCurCount_ = 0;

	lightSpotMaxCount_ = LIGHT_START_MAX_COUNT_SPOT;
	lightSpotCurCount_ = 0;

	lightDirs_		= std::vector<LightDescDir>(lightDirMaxCount_);
	lightPoints_	= std::vector<LightDescPoint>(lightPointMaxCount_);
	lightSpots_		= std::vector<LightDescSpot>(lightSpotMaxCount_);

	lightDirBuffer_		= nullptr;
	lightDirSRV_		= nullptr;

	lightPointBuffer_	= nullptr;
	lightPointSRV_		= nullptr;
	
	lightSpotBuffer_	= nullptr;
	lightSpotSRV_		= nullptr;

	attributesPosition_ = nullptr;

	ATTRIBUTES_INIT_ALL;
}
ManagementLight::~ManagementLight()
{
	SAFE_RELEASE(lightDirBuffer_);
	SAFE_RELEASE(lightDirSRV_);
	SAFE_RELEASE(lightPointBuffer_);
	SAFE_RELEASE(lightPointSRV_);
	SAFE_RELEASE(lightSpotBuffer_);
	SAFE_RELEASE(lightSpotSRV_);
}

void ManagementLight::reset()
{
	SAFE_RELEASE(lightDirBuffer_);
	SAFE_RELEASE(lightDirSRV_);
	SAFE_RELEASE(lightPointBuffer_);
	SAFE_RELEASE(lightPointSRV_);
	SAFE_RELEASE(lightSpotBuffer_);
	SAFE_RELEASE(lightSpotSRV_);
}
HRESULT ManagementLight::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	attributesPosition_	= GET_ATTRIBUTES(position);

	hr = initLightDir(device);
	if(SUCCEEDED(hr))
		hr = initLightPoint(device);
	if(SUCCEEDED(hr))
		hr = initLightSpot(device);

	return hr;
}

HRESULT ManagementLight::initLightDir(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initLightDirBuffer(device);
	if(SUCCEEDED(hr))
		hr = initLightDirSRV(device);

	return hr;
}
HRESULT ManagementLight::initLightDirBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightDirCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth			= sizeof(LightDescDir) * lightDirMaxCount_;
		bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(LightDescDir);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lightDirs_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightDirBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightDirBuffer device->CreateBuffer failed!");
	}

	return hr;
}
HRESULT ManagementLight::initLightDirSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightDirCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		lightDirBuffer_->GetDesc(&bufferDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));

		srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement	= 0;
		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		hr = device->CreateShaderResourceView(lightDirBuffer_, &srvDesc, &lightDirSRV_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightDirSRV device->CreateShaderResourceView failed!");
	}

	return hr;
}

HRESULT ManagementLight::initLightPoint(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initLightPointBuffer(device);
	if(SUCCEEDED(hr))
		hr = initLightPointSRV(device);

	return hr;
}
HRESULT ManagementLight::initLightPointBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightPointCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth			= sizeof(LightDescPoint) * lightPointMaxCount_;
		bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(LightDescPoint);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lightPoints_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightPointBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightPointBuffer device->CreateBuffer failed!");
	}

	return hr;
}
HRESULT ManagementLight::initLightPointSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightPointCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		lightPointBuffer_->GetDesc(&bufferDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));

		srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement	= 0;
		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		hr = device->CreateShaderResourceView(lightPointBuffer_, &srvDesc, &lightPointSRV_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightPointSRV device->CreateShaderResourceView failed!");
	}

	return hr;
}

HRESULT ManagementLight::initLightSpot(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initLightSpotBuffer(device);
	if(SUCCEEDED(hr))
		hr = initLightSpotSRV(device);

	return hr;
}
HRESULT ManagementLight::initLightSpotBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightSpotCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth			= sizeof(LightDescSpot) * lightSpotMaxCount_;
		bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(LightDescSpot);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lightSpots_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightSpotBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightSpotBuffer device->CreateBuffer failed!");
	}

	return hr;
}
HRESULT ManagementLight::initLightSpotSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(lightSpotCurCount_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		lightSpotBuffer_->GetDesc(&bufferDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));

		srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement	= 0;
		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		hr = device->CreateShaderResourceView(lightSpotBuffer_, &srvDesc, &lightSpotSRV_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightSpotSRV device->CreateShaderResourceView failed!");
	}

	return hr;
}

void ManagementLight::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	updateLightDir(device, devcon);
	updateLightPoint(device, devcon);
	updateLightSpot(device, devcon);
}

void ManagementLight::setLightSRVCS(
	ID3D11DeviceContext*	devcon, 
	LightDescType			lightType, 
	unsigned int			shaderRegister)
{
	switch (lightType)
	{
	case LIGHTDESCTYPE_DIR:
		devcon->CSSetShaderResources(shaderRegister, 1, &lightDirSRV_);
		break;
	case LIGHTDESCTYPE_POINT:
		devcon->CSSetShaderResources(shaderRegister, 1, &lightPointSRV_);
		break;
	case LIGHTDESCTYPE_SPOT:
		devcon->CSSetShaderResources(shaderRegister, 1, &lightSpotSRV_);
		break;
	}
}

void ManagementLight::updateLightDir(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	Attribute_Light_Dir* lightDirAt;

	lightDirCurCount_	= 0;
	while(itrLightDir.hasNext())
	{
		lightDirAt	= itrLightDir.getNext();

		if(lightDirCurCount_ < lightDirs_.size())
			lightDirs_[lightDirCurCount_] = lightDirAt->lightDir;
		else
			lightDirs_.push_back(lightDirAt->lightDir);

		lightDirCurCount_++;
	}

	if(lightDirCurCount_ > 0)
	{
		if(lightDirCurCount_ > lightDirMaxCount_)
		{ //resize buffer
			increaseLightCapacity(device, LIGHTDESCTYPE_DIR);
		}
		else
		{ //simply map to buffer
			if(!lightDirBuffer_)
				increaseLightCapacity(device, LIGHTDESCTYPE_DIR);
			updateLightBuffers(devcon, LIGHTDESCTYPE_DIR);
		}
	}
}
void ManagementLight::updateLightPoint(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	Attribute_Light_Point*	lightPointAt;
	Attribute_Position*		posAt;

	lightPointCurCount_	= 0;
	while(itrLightPoint.hasNext())
	{
		lightPointAt = itrLightPoint.getNext();
		posAt = &attributesPosition_->at(lightPointAt->ptr_position.index);
		lightPointAt->lightPoint.pos = posAt->position;

		if(lightPointCurCount_ < lightPoints_.size())
			lightPoints_[lightPointCurCount_] = lightPointAt->lightPoint;
		else
			lightPoints_.push_back(lightPointAt->lightPoint);

		lightPointCurCount_++;
	}

	if(lightPointCurCount_ > 0)
	{
		if(lightPointCurCount_ > lightPointMaxCount_)
		{ //resize buffer
			increaseLightCapacity(device, LIGHTDESCTYPE_POINT);
		}
		else
		{ //simply map to buffer
			if(!lightPointBuffer_)
				increaseLightCapacity(device, LIGHTDESCTYPE_POINT);
			updateLightBuffers(devcon, LIGHTDESCTYPE_POINT);
		}
	}
}
void ManagementLight::updateLightSpot(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	Attribute_Light_Spot*	lightSpotAt;
	Attribute_Position*		posAt;

	lightSpotCurCount_ = 0;
	while(itrLightSpot.hasNext())
	{
		lightSpotAt = itrLightSpot.getNext();
		posAt = &attributesPosition_->at(lightSpotAt->ptr_position.index);
		lightSpotAt->lightSpot.pos = posAt->position;

		if(lightSpotCurCount_ < lightSpots_.size())
			lightSpots_[lightSpotCurCount_] = lightSpotAt->lightSpot;
		else
			lightSpots_.push_back(lightSpotAt->lightSpot);

		lightSpotCurCount_++;
	}

	if(lightSpotCurCount_ > 0)
	{
		if(lightSpotCurCount_ > lightSpotMaxCount_)
		{ //resize buffer
			increaseLightCapacity(device, LIGHTDESCTYPE_SPOT);
		}
		else
		{ //simply map to buffer
			if(!lightSpotBuffer_)
				increaseLightCapacity(device, LIGHTDESCTYPE_SPOT);
			updateLightBuffers(devcon, LIGHTDESCTYPE_SPOT);
		}
	}
}

HRESULT ManagementLight::increaseLightCapacity(ID3D11Device* device, LightDescType lightType)
{
	HRESULT hr = S_OK;

	switch(lightType)
	{
	case LIGHTDESCTYPE_DIR:
		lightDirMaxCount_ *= 2;
		SAFE_RELEASE(lightDirBuffer_);
		SAFE_RELEASE(lightDirSRV_);
		hr = initLightDir(device);
		break;
	case LIGHTDESCTYPE_POINT:
		lightPointMaxCount_ *= 2;
		SAFE_RELEASE(lightPointBuffer_);
		SAFE_RELEASE(lightPointSRV_);
		hr = initLightPoint(device);
		break;
	case LIGHTDESCTYPE_SPOT:
		lightSpotMaxCount_ *= 2;
		SAFE_RELEASE(lightSpotBuffer_);
		SAFE_RELEASE(lightSpotSRV_);
		hr = initLightSpot(device);
		break;
	default:
		hr = S_FALSE;
		break;
	}

	return hr;
}
HRESULT ManagementLight::updateLightBuffers(ID3D11DeviceContext* devcon, LightDescType lightType)
{
	HRESULT hr = S_OK;

	ID3D11Buffer* lightBuffer = nullptr;
	switch (lightType)
	{
	case LIGHTDESCTYPE_DIR:
		lightBuffer = lightDirBuffer_;
		break;
	case LIGHTDESCTYPE_POINT:
		lightBuffer = lightPointBuffer_;
		break;
	case LIGHTDESCTYPE_SPOT:
		lightBuffer = lightSpotBuffer_;
		break;
	default:
		hr = S_FALSE;
		break;
	}

	D3D11_MAPPED_SUBRESOURCE map;
	hr = devcon->Map(
		lightBuffer, 
		NULL, 
		D3D11_MAP_WRITE_DISCARD, 
		NULL, 
		&map);
	if(SUCCEEDED(hr))
	{
		switch (lightType)
		{
		case LIGHTDESCTYPE_DIR:
			memcpy(map.pData, &lightDirs_[0], lightDirCurCount_ * sizeof(LightDescDir));
			break;
		case LIGHTDESCTYPE_POINT:
			memcpy(map.pData, &lightPoints_[0], lightPointCurCount_ * sizeof(LightDescPoint));
			break;
		case LIGHTDESCTYPE_SPOT:
			memcpy(map.pData, &lightSpots_[0], lightSpotCurCount_ * sizeof(LightDescSpot));
			break;
		default:
			break;
		}

		devcon->Unmap(lightBuffer, NULL);
	}
	else
		ERROR_MSG(L"RendererLight::LightManagement::updateLightBuffers devcon->Map failed!");

	return hr;
}

unsigned int ManagementLight::getLightDirCurCount()		const
{
	return lightDirCurCount_;
}
unsigned int ManagementLight::getLightPointCurCount()	const
{
	return lightPointCurCount_;
}
unsigned int ManagementLight::getLightSpotCurCount()	const
{
	return lightSpotCurCount_;
}