
#include <d3d11.h>

#include "LightManagement.h"
#include "renderingUtilities.h"


LightManagement::LightManagement()
{
	maxNumLights_	= 20;
	numLights_		= 0;
	lights_.resize(maxNumLights_);

	lightBuffer_ = nullptr;
	lightSRV_	= nullptr;
}

LightManagement::~LightManagement()
{
	SAFE_RELEASE(lightBuffer_);
	SAFE_RELEASE(lightSRV_);
}

void LightManagement::reset()
{
	SAFE_RELEASE(lightBuffer_);
	SAFE_RELEASE(lightSRV_);
}

HRESULT LightManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	createDirectionalLight(device);
	
	hr = createLightBuffer(device);
	if(SUCCEEDED(hr))
		hr = createLightSRV(device);

	return hr;
}

HRESULT LightManagement::updateBufferData(ID3D11DeviceContext* devcon)
{
	HRESULT hr = S_OK;
	if(lights_.size() == 0)
		hr = E_FAIL;
	if(SUCCEEDED(hr))
	{
		D3D11_MAPPED_SUBRESOURCE map;
		hr = devcon->Map(lightBuffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &map);
		if(SUCCEEDED(hr))
		{
			memcpy(map.pData, &lights_[0], lights_.size() * sizeof(LightDesc));
			devcon->Unmap(lightBuffer_, NULL);
		}
	}

	return hr;
}

void LightManagement::setLightSRVCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
{
	devcon->CSSetShaderResources(shaderRegister, 1, &lightSRV_);
}

void LightManagement::addLight(LightDesc light, ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	if(numLights_ == maxNumLights_)
		resizeLights(device);
	lights_[numLights_] = light;
	numLights_++;

	updateBufferData(devcon);
}

void LightManagement::createDirectionalLight(ID3D11Device* device)
{
	if(numLights_ == maxNumLights_)
		resizeLights( device);

	LightDesc lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));

	lightDesc.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	lightDesc.ambient	= DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	lightDesc.diffuse	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.specular	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.type		= LIGHT_DIRECTIONAL;

	lights_[numLights_] = lightDesc;
	numLights_++;
}

void LightManagement::resizeLights(ID3D11Device* device)
{
	maxNumLights_ *= 2;
	lights_.resize(maxNumLights_);

	SAFE_RELEASE(lightBuffer_);
	SAFE_RELEASE(lightSRV_);

	createLightBuffer(device);
	createLightSRV(device);
}

HRESULT LightManagement::createLightBuffer(ID3D11Device* device)
{
	HRESULT hr = E_FAIL;
	if(lights_.size() > 0)
		hr = S_OK;
	else
		ERROR_MSG(L"RendererLight::LightManagement::createLightBuffer failed! The vector lights is empty");

	if(SUCCEEDED(hr))
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth = sizeof(LightDesc) * lights_.size();
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride = sizeof(LightDesc);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lights_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightBuffer device->CreateBuffer failed!");
	}
	return hr;
}

HRESULT LightManagement::createLightSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		lightBuffer_->GetDesc(&bufferDesc);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		
		srvDesc.ViewDimension	= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.Format	= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

	hr = device->CreateShaderResourceView(lightBuffer_, &srvDesc, &lightSRV_);
	if(FAILED(hr))
		ERROR_MSG(L"RendererLight::LightManagement::createLightSRV device->CreateShaderResourceView failed!");

	return hr;
}