
#include "CBManagement.h"
#include "CBDesc.h"
#include "renderingUtilities.h"

CBManagement::CBManagement()
{
	cbFrame_	= nullptr;
	cbInstance_ = nullptr;
}
CBManagement::~CBManagement()
{
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbInstance_);
}
void CBManagement::reset()
{
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbInstance_);
}

void CBManagement::updateCBFrame(ID3D11DeviceContext* devcon,
								 DirectX::XMFLOAT4X4 finalMatrix,
								 DirectX::XMFLOAT4X4 viewMatrix,
								 DirectX::XMFLOAT4X4 viewMatrixInverse,
								 DirectX::XMFLOAT4X4 projectionMatrix,
								 DirectX::XMFLOAT4X4 projectionMatrixInverse,
								 DirectX::XMFLOAT3	 eyePosition,
								 unsigned int		 numLights)
{
	CBFrameDesc cbDesc;
	cbDesc.finalMatrix				= finalMatrix;
	cbDesc.viewMatrix				= viewMatrix;
	cbDesc.viewMatrixInverse		= viewMatrixInverse;
	cbDesc.projectionMatrix			= projectionMatrix;
	cbDesc.projectionMatrixInverse	= projectionMatrixInverse;
	cbDesc.eyePosition				= eyePosition;
	cbDesc.numLights				= numLights;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBInstance(ID3D11DeviceContext* devcon,
									unsigned int screenWidth,
									unsigned int screenHeight)
{
	CBInstanceDesc cbDesc;
	cbDesc.screenWidth  = screenWidth;
	cbDesc.screenHeight = screenHeight;

	devcon->UpdateSubresource(cbInstance_, 0, 0, &cbDesc, 0, 0);
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