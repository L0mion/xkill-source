
#include <d3d11.h>

#include "LightManagement.h"
#include "renderingUtilities.h"


RendererLight::LightManagement::LightManagement()
{
	lightBuffer = nullptr;
	lightUAV	= nullptr;
}

RendererLight::LightManagement::~LightManagement()
{
	SAFE_RELEASE(lightBuffer);
	SAFE_RELEASE(lightUAV);
}

void RendererLight::LightManagement::reset()
{
	SAFE_RELEASE(lightBuffer);
	SAFE_RELEASE(lightUAV);
}

void RendererLight::LightManagement::init(ID3D11Device* device)
{
	createDirectionalLight();
	createLightBuffer(device);
	createLightUAV(device);
}

void RendererLight::LightManagement::setLightUAVCS(ID3D11DeviceContext* devcon)
{
	devcon->CSGetUnorderedAccessViews(0, 1, &lightUAV);
}

void RendererLight::LightManagement::createDirectionalLight()
{
	RendererLight::LightDesc lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));

	lightDesc.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	lightDesc.ambient	= DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	lightDesc.diffuse	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.specular	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.type		= RendererLight::LIGHT_DIRECTIONAL;

	lights.push_back(lightDesc);
}

HRESULT RendererLight::LightManagement::createLightBuffer(ID3D11Device* device)
{
	HRESULT hr = E_FAIL;
	if(lights.size() > 0)
		hr = S_OK;
	else
		ERROR_MSG(L"RendererLight::LightManagement::createLightBuffer failed! The vector lights is empty");

	if(SUCCEEDED(hr))
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth = sizeof(RendererLight::LightDesc) * lights.size();
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(RendererLight::LightDesc);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lights.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightBuffer);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightBuffer device->CreateBuffer failed!");
	}
	return hr;
}

HRESULT RendererLight::LightManagement::createLightUAV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	lightBuffer->GetDesc(&bufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

	hr = device->CreateUnorderedAccessView(lightBuffer, &uavDesc, &lightUAV);
	if(FAILED(hr))
		ERROR_MSG(L"RendererLight::LightManagement::createLightUAV device->CreateUnorderedAccessView failed!");

	return hr;
}