
#include "ManagementCB.h"
#include "CBDesc.h"
#include "renderingUtilities.h"

ManagementCB::ManagementCB()
{
	cbInstance_ = nullptr;
	cbFrame_	= nullptr;
	cbCamera_	= nullptr;
	cbObject_	= nullptr;
	cbSubset_	= nullptr;
	cbBone_		= nullptr;
}
ManagementCB::~ManagementCB()
{
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbCamera_);
	SAFE_RELEASE(cbObject_);
	SAFE_RELEASE(cbSubset_);
	SAFE_RELEASE(cbBone_);
}
void ManagementCB::reset()
{
	SAFE_RELEASE(cbInstance_);
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbCamera_);
	SAFE_RELEASE(cbObject_);
	SAFE_RELEASE(cbSubset_);
	SAFE_RELEASE(cbBone_);
}

void ManagementCB::updateCBInstance(ID3D11DeviceContext*	devcon,
									const unsigned int		screenWidth,
									const unsigned int		screenHeight)
{
	CBInstanceDesc cbDesc;
	cbDesc.screenWidth_		= screenWidth;
	cbDesc.screenHeight_	= screenHeight;

	devcon->UpdateSubresource(cbInstance_, 0, 0, &cbDesc, 0, 0);
}
void ManagementCB::updateCBFrame(ID3D11DeviceContext* devcon, unsigned int numLights)
{
	CBFrameDesc cbDesc;
	cbDesc.numLights_ = numLights;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cbDesc, 0, 0);
}
void ManagementCB::updateCBCamera(ID3D11DeviceContext*	devcon,
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
void ManagementCB::updateCBObject(ID3D11DeviceContext* devcon,
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
void ManagementCB::updateCBSubset(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT3	specularTerm,
						float				specularPower)
{
	CBSubsetDesc cbDesc;
	cbDesc.specularTerm_	= specularTerm;
	cbDesc.specularPower_	= specularPower;

	devcon->UpdateSubresource(cbSubset_, 0, 0, &cbDesc, 0, 0);
}
void ManagementCB::updateCBBone(ID3D11DeviceContext* devcon, std::vector<DirectX::XMFLOAT4X4> boneTransforms)
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

void ManagementCB::setCB(
	CB_TYPE					cbType, 
	TypeFX					shaderStage, 
	unsigned int			shaderRegister, 
	ID3D11DeviceContext*	devcon)
{
	//Get specified buffer
	ID3D11Buffer* cb = nullptr;
	switch(cbType)
	{
	case CB_TYPE_INSTANCE:
		cb = cbInstance_;
		break;
	case CB_TYPE_FRAME:
		cb = cbFrame_;
		break;
	case CB_TYPE_CAMERA:
		cb = cbCamera_;
		break;
	case CB_TYPE_OBJECT:
		cb = cbObject_;
		break;
	case CB_TYPE_SUBSET:
		cb = cbSubset_;
		break;
	case CB_TYPE_BONE:
		cb = cbBone_;
		break;
	}

	//Set to specified shader-stage
	switch(shaderStage)
	{
	case TypeFX_VS:
		devcon->VSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	case TypeFX_GS:
		devcon->GSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	case TypeFX_HS:
		devcon->HSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	case TypeFX_DS:
		devcon->DSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	case TypeFX_PS:
		devcon->PSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	case TypeFX_CS:
		devcon->CSSetConstantBuffers(shaderRegister, 1, &cb);
		break;
	}
}

HRESULT ManagementCB::init(ID3D11Device* device)
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
HRESULT ManagementCB::initCBInstance(ID3D11Device* device)
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
HRESULT ManagementCB::initCBFrame(ID3D11Device* device)
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
HRESULT ManagementCB::initCBCamera(ID3D11Device* device)
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
HRESULT ManagementCB::initCBObject(ID3D11Device* device)
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
HRESULT ManagementCB::initCBSubset(ID3D11Device* device)
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
HRESULT ManagementCB::initCBBone(ID3D11Device* device)
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