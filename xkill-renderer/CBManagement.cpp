
#include "CBManagement.h"
#include "CBDesc.h"
#include "renderingUtilities.h"

CBManagement::CBManagement()
{
	cbInstance_ = nullptr;
	cbFrame_	= nullptr;
	cbCamera_	= nullptr;
	cbObject_	= nullptr;
	cbSubset_	= nullptr;
	cbBone_		= nullptr;
}
CBManagement::~CBManagement()
{
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbCamera_);
	SAFE_RELEASE(cbObject_);
	SAFE_RELEASE(cbSubset_);
	SAFE_RELEASE(cbBone_);
}
void CBManagement::reset()
{
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbCamera_);
	SAFE_RELEASE(cbObject_);
	SAFE_RELEASE(cbSubset_);
	SAFE_RELEASE(cbBone_);
}

void CBManagement::updateCBInstance(ID3D11DeviceContext*	devcon,
									const unsigned int		screenWidth,
									const unsigned int		screenHeight)
{
	CBInstanceDesc cbDesc;
	cbDesc.screenWidth_		= screenWidth;
	cbDesc.screenHeight_	= screenHeight;

	devcon->UpdateSubresource(cbInstance_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBFrame(ID3D11DeviceContext* devcon, unsigned int numLights)
{
	CBFrameDesc cbDesc;
	cbDesc.numLights_ = numLights;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBCamera(ID3D11DeviceContext*	devcon,
								  DirectX::XMFLOAT4X4	viewMatrix,
								  DirectX::XMFLOAT4X4	viewMatrixInverse,
								  DirectX::XMFLOAT4X4	projectionMatrix,
								  DirectX::XMFLOAT4X4	projectionMatrixInverse,
								  DirectX::XMFLOAT3		eyePosition,
								  unsigned int			viewportTopX,
								  unsigned int			viewportTopY)
{
	CBCameraDesc cbDesc;
	cbDesc.viewMatrix_				= viewMatrix;
	cbDesc.viewMatrixInverse_		= viewMatrixInverse;
	cbDesc.projectionMatrix_		= projectionMatrix;
	cbDesc.projectionMatrixInverse_	= projectionMatrixInverse;
	cbDesc.eyePosition_				= eyePosition;
	cbDesc.viewportTopX_			= viewportTopX;
	cbDesc.viewportTopY_			= viewportTopY;

	devcon->UpdateSubresource(cbCamera_, 0, 0, &cbDesc, 0, 0);
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
void CBManagement::updateCBSubset(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT3	specularTerm,
						float				specularPower)
{
	CBSubsetDesc cbDesc;
	cbDesc.specularTerm_	= specularTerm;
	cbDesc.specularPower_	= specularPower;

	devcon->UpdateSubresource(cbSubset_, 0, 0, &cbDesc, 0, 0);
}
void CBManagement::updateCBBone(ID3D11DeviceContext* devcon, std::vector<DirectX::XMFLOAT4X4> boneTransforms)
{
	unsigned int numBones = boneTransforms.size();
	if(numBones > CB_BONE_DESC_NUM_BONES)
		numBones = CB_BONE_DESC_NUM_BONES;

	CBBoneDesc cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	for(unsigned int i=0; i<numBones; i++)
		cbDesc.boneTransforms[i] = boneTransforms[i];

	devcon->UpdateSubresource(cbBone_, 0, 0, &cbDesc, 0, 0);
}

void CBManagement::vsSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbType)
	{
	case CB_TYPE_INSTANCE:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_TYPE_FRAME:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_TYPE_CAMERA:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbCamera_);
		break;
	case CB_TYPE_OBJECT:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	case CB_TYPE_SUBSET:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbSubset_);
		break;
	case CB_TYPE_BONE:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cbBone_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}
void CBManagement::psSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbType)
	{
	case CB_TYPE_INSTANCE:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_TYPE_FRAME:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_TYPE_CAMERA:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbCamera_);
		break;
	case CB_TYPE_OBJECT:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	case CB_TYPE_SUBSET:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbSubset_);
		break;
	case CB_TYPE_BONE:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cbBone_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}
void CBManagement::csSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon)
{
	switch(cbType)
	{
	case CB_TYPE_INSTANCE:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbInstance_);
		break;
	case CB_TYPE_FRAME:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbFrame_);
		break;
	case CB_TYPE_CAMERA:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbCamera_);
		break;
	case CB_TYPE_OBJECT:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbObject_);
		break;
	case CB_TYPE_SUBSET:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbSubset_);
		break;
	case CB_TYPE_BONE:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cbBone_);
		break;
	default:
		ERROR_MSG(L"CBManagement::vsSet | Failed! | Index not recognized!");
		break;
	}
}

HRESULT CBManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initCBInstance(device);
	if(SUCCEEDED(hr))
		hr = initCBFrame(device);
	if(SUCCEEDED(hr))
		hr = initCBCamera(device);
	if(SUCCEEDED(hr))
		hr = initCBObject(device);
	if(SUCCEEDED(hr))
		hr = initCBSubset(device);
	if(SUCCEEDED(hr))
		hr = initCBBone(device);

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
HRESULT CBManagement::initCBCamera(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_CAMERA_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbCamera_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBCamera | device->CreateBuffer | Failed!");

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
HRESULT CBManagement::initCBSubset(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_SUBSET_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbSubset_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBSubset | device->CreateBuffer | Failed!");
		
	return hr;
}
HRESULT CBManagement::initCBBone(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_BONE_DESC_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &cbBone_);
	if(FAILED(hr))
		ERROR_MSG(L"CBManagement::initCBBone | device->CreateBuffer | Failed!");

	return hr;
}
