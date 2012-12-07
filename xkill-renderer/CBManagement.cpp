
#include "CBManagement.h"
#include "CBDesc.h"
#include "renderingUtilities.h"

CBManagement::CBManagement()
{
	cbFrame_	= nullptr;
	cbInstance_ = nullptr;
	cbObject_	= nullptr;
}
CBManagement::~CBManagement()
{
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbObject_);
}
void CBManagement::reset()
{
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbObject_);
}

void CBManagement::updateCBFrame(ID3D11DeviceContext* devcon,
								 DirectX::XMFLOAT4X4 viewMatrix,
								 DirectX::XMFLOAT4X4 viewMatrixInverse,
								 DirectX::XMFLOAT4X4 projectionMatrix,
								 DirectX::XMFLOAT4X4 projectionMatrixInverse,
								 DirectX::XMFLOAT3	 eyePosition,
								 unsigned int		 numLights)
{
	CBFrameDesc cbDesc;
	cbDesc.viewMatrix_				= viewMatrix;
	cbDesc.viewMatrixInverse_		= viewMatrixInverse;
	cbDesc.projectionMatrix_		= projectionMatrix;
	cbDesc.projectionMatrixInverse_	= projectionMatrixInverse;
	cbDesc.eyePosition_				= eyePosition;
	cbDesc.numLights_				= numLights;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBInstance(
	ID3D11DeviceContext*	devcon,
	const unsigned int		screenWidth,
	const unsigned int		screenHeight,
	const unsigned int		tileWidth,
	const unsigned int		tileHeight)
{
	CBInstanceDesc cbDesc;
	cbDesc.screenWidth_		= screenWidth;
	cbDesc.screenHeight_	= screenHeight;
	cbDesc.tileWidth_		= tileWidth;
	cbDesc.tileHeight_		= tileHeight;

	devcon->UpdateSubresource(cbInstance_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBObject(ID3D11DeviceContext* devcon,
								  DirectX::XMFLOAT4X4 finalMatrix,
								  DirectX::XMFLOAT4X4 worldMatrix,
								  DirectX::XMFLOAT4X4 worldMatrixInverse)
{
	CBObjectDesc cbDesc;
	cbDesc.finalMatrix_			= finalMatrix;
	cbDesc.worldMatrix_			= worldMatrix;
	cbDesc.worldMatrixInverse_	= worldMatrixInverse;

	devcon->UpdateSubresource(cbObject_, 0, 0, &cbDesc, 0, 0);
}

void CBManagement::vsSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbIndex)
	{
	case CB_FRAME_INDEX:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_INSTANCE_INDEX:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_OBJECT_INDEX:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}
void CBManagement::psSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbIndex)
	{
	case CB_FRAME_INDEX:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_INSTANCE_INDEX:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_OBJECT_INDEX:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}
void CBManagement::csSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbIndex)
	{
	case CB_FRAME_INDEX:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_INSTANCE_INDEX:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_OBJECT_INDEX:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}

HRESULT CBManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initCBFrame(device);
	if(SUCCEEDED(hr))
		hr = initCBInstance(device);
	if(SUCCEEDED(hr))
		hr = initCBObject(device);

	return hr;
}
HRESULT CBManagement::initCBFrame(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_FRAME_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbFrame_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBFrame | device->CreateBuffer | Failed!");

	return hr;
}
HRESULT CBManagement::initCBInstance(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_INSTANCE_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbInstance_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBInstance | device->CreateBuffer | Failed!");

	return hr;
}
HRESULT CBManagement::initCBObject(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_OBJECT_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbObject_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBObject | device->CreateBuffer | Failed!");

	return hr;
}