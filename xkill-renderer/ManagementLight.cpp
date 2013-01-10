#include <d3d11.h>

#include <xkill-utilities/EventManager.h>

#include "ManagementLight.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementLight::ManagementLight()
{
	maxNumLights_	= 5;
	curNumLights_	= 0;

	//numLights_		= 0;
	//lights_.resize(maxNumLights_);

	attributesPosition_ = nullptr;

	lightBuffer_	= nullptr;
	lightSRV_		= nullptr;

	ATTRIBUTES_INIT_ALL;
}

ManagementLight::~ManagementLight()
{
	SAFE_RELEASE(lightBuffer_);
	SAFE_RELEASE(lightSRV_);
}

void ManagementLight::reset()
{
	SAFE_RELEASE(lightBuffer_);
	SAFE_RELEASE(lightSRV_);
}

HRESULT ManagementLight::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	attributesPosition_	= GET_ATTRIBUTES(position);

	//createDirectionalLight(device);

	hr = initLightBuffer(device);
	if(SUCCEEDED(hr))
		hr = createLightSRV(device);

	return hr;
}

void ManagementLight::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	Attribute_Light*	lightAt;
	Attribute_Position*	posAt;

	LightDesc lightDesc;
	lights_ = std::vector<LightDesc>(maxNumLights_);

	curNumLights_ = 0;
	while(itrLight.hasNext())
	{
		lightAt = itrLight.getNext();
		lightDesc.range			= lightAt->range;
		lightDesc.direction		= lightAt->direction;
		lightDesc.spotPower		= lightAt->spotPower;
		lightDesc.ambient		= lightAt->ambient;
		lightDesc.diffuse		= lightAt->diffuse;
		lightDesc.specular		= lightAt->specular;
		lightDesc.attenuation	= lightAt->attenuation;
		lightDesc.type			= lightAt->lightType;

		posAt = &attributesPosition_->at(lightAt->ptr_position.index);
		lightDesc.position = posAt->position;

		if(curNumLights_ < lights_.size())
			lights_[curNumLights_] = lightDesc;
		else
			lights_.push_back(lightDesc);
		
		curNumLights_++;
	}

	if(curNumLights_ > 0)
	{
		if(lightBuffer_ == nullptr)
			resizeLights(device);

		if(curNumLights_ > maxNumLights_)
		{ //resize buffer
			resizeLights(device);
		}
		else
		{ //simply map to buffer
			updateBufferData(devcon);
		}
	}
}

HRESULT ManagementLight::updateBufferData(ID3D11DeviceContext* devcon)
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
			memcpy(map.pData, &lights_[0], curNumLights_ * sizeof(LightDesc));
			devcon->Unmap(lightBuffer_, NULL);
		}
	}

	return hr;
}

void ManagementLight::setLightSRVCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
{
	devcon->CSSetShaderResources(shaderRegister, 1, &lightSRV_);
}

void ManagementLight::addLight(LightDesc light, ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	if(curNumLights_ == maxNumLights_)
		resizeLights(device);
	lights_[curNumLights_] = light;
	curNumLights_++;

	updateBufferData(devcon);
}

/*
void ManagementLight::createDirectionalLight(ID3D11Device* device)
{
	if(numLights_ == maxNumLights_)
		resizeLights( device);

	LightDesc lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	
	lightDesc.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	lightDesc.ambient	= DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	lightDesc.diffuse	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.specular	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.type		= LIGHT_DIRECTIONAL;
	
	lights_[numLights_] = lightDesc;
	numLights_++;
}
*/

void ManagementLight::resizeLights(ID3D11Device* device)
{
	maxNumLights_ *= 2; //double capacity
	//lights_.resize(maxNumLights_);

	SAFE_RELEASE(lightBuffer_);
	lightBuffer_ = nullptr;
	SAFE_RELEASE(lightSRV_);
	lightSRV_ = nullptr;

	initLightBuffer(device);
	createLightSRV(device);
}

HRESULT ManagementLight::initLightBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	
	if(curNumLights_ > 0)
	{ //Create buffer, otherwize don't bother.
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth			= sizeof(LightDesc) * maxNumLights_;
		bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(LightDesc);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &lights_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &lightBuffer_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightBuffer device->CreateBuffer failed!");
	}

	/*
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
	*/

	return hr;
}
HRESULT ManagementLight::createLightSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	if(curNumLights_ > 0)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		lightBuffer_->GetDesc(&bufferDesc);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		
		srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement	= 0;
		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		hr = device->CreateShaderResourceView(lightBuffer_, &srvDesc, &lightSRV_);
		if(FAILED(hr))
			ERROR_MSG(L"RendererLight::LightManagement::createLightSRV device->CreateShaderResourceView failed!");
	}

	return hr;
}

unsigned int ManagementLight::getNumLights() const
{
	return curNumLights_;
}