#include "fxManagement.h"
#include "renderingUtilities.h"

FXManagement::FXManagement()
{
	defaultVS			= nullptr;
	defaultPS			= nullptr;
	defaultDeferredVS	= nullptr;
	defaultDeferredPS	= nullptr;
	inputLayout			= nullptr;

	blobDefaultVS			= nullptr;
	blobDefaultPS			= nullptr;
	blobDefaultDeferredVS	= nullptr;
	blobDefaultDeferredPS	= nullptr;
}

FXManagement::~FXManagement()
{
}

void FXManagement::cleanUp()
{
	SAFE_RELEASE(defaultVS);
	SAFE_RELEASE(defaultPS);
	SAFE_RELEASE(defaultDeferredVS);
	SAFE_RELEASE(defaultDeferredPS);
	
	SAFE_RELEASE(inputLayout);
	
	SAFE_RELEASE(blobDefaultVS);
	SAFE_RELEASE(blobDefaultPS);
	SAFE_RELEASE(blobDefaultDeferredVS);
	SAFE_RELEASE(blobDefaultDeferredPS);
}

HRESULT FXManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initShaders(device);
	if(SUCCEEDED(hr))
		hr = initInputLayout(device);

	return hr;
}

HRESULT FXManagement::initShaders(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initDefaultVS(device);
	if(SUCCEEDED(hr))
		hr = initDefaultPS(device);
	if(SUCCEEDED(hr))
		hr = initDefaultDeferredVS(device);
	if(SUCCEEDED(hr))
		hr = initDefaultDeferredPS(device);

	return hr;
}

HRESULT FXManagement::initDefaultVS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	
	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultVS.cso", &blobDefaultVS);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initDefaultVS D3DReadFileToBlob failed");
	else
	{
		hr = device->CreateVertexShader(
			blobDefaultVS->GetBufferPointer(),
			blobDefaultVS->GetBufferSize(),
			nullptr,
			&defaultVS);
		if(FAILED(hr))
			ERROR_MSG(L"FXManagement::initDefaultVS CreateVertexShader failed");
	}

	return hr;
}

HRESULT FXManagement::initDefaultPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultPS.cso", &blobDefaultPS);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initDefaultPS D3DReadFileToBlob failed");
	if(hr == S_OK)
	{
		hr = device->CreatePixelShader(
			blobDefaultPS->GetBufferPointer(),
			blobDefaultPS->GetBufferSize(),
			nullptr,
			&defaultPS);
		if(FAILED(hr))
			ERROR_MSG(L"FXManagement::initDefaultPS CreatePixelShader failed");
	}

	return hr;
}

HRESULT FXManagement::initDefaultDeferredVS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	
	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultDeferredVS.cso", &blobDefaultDeferredVS);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initDefaultDeferredVS D3DReadFileToBlob failed");
	if(hr == S_OK)
	{
		hr = device->CreateVertexShader(
			blobDefaultDeferredVS->GetBufferPointer(),
			blobDefaultDeferredVS->GetBufferSize(),
			nullptr,
			&defaultDeferredVS);
		if(FAILED(hr))
			ERROR_MSG(L"FXManagement::initDefaultDeferredVS CreateVertexShader failed");
	}

	return hr;
}

HRESULT FXManagement::initDefaultDeferredPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultDeferredPS.cso", &blobDefaultDeferredPS);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initDefaultDeferredPS D3DReadFileToBlob failed");
	if(hr == S_OK)
	{
		hr = device->CreatePixelShader(
			blobDefaultDeferredPS->GetBufferPointer(),
			blobDefaultDeferredPS->GetBufferSize(),
			nullptr,
			&defaultDeferredPS);
		if(FAILED(hr))
			ERROR_MSG(L"FXManagement::initDefaultDeferredPS CreatePixelShader failed");
	}

	return hr;
}

HRESULT FXManagement::initInputLayout(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = device->CreateInputLayout(
		ied, 
		3, 
		blobDefaultVS->GetBufferPointer(), 
		blobDefaultVS->GetBufferSize(), 
		&inputLayout);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initInputLayout CreateInputLayout failed");
	return hr;
}

ID3D11VertexShader* FXManagement::getDefaultVS() const
{
	return defaultVS;
}

ID3D11PixelShader* FXManagement::getDefaultPS() const
{
	return defaultPS;
}

ID3D11VertexShader* FXManagement::getDefaultDeferredVS()	const
{
	return defaultDeferredVS;
}

ID3D11PixelShader* FXManagement::getDefaultDeferredPS() const
{
	return defaultDeferredPS;
}

ID3D11InputLayout* FXManagement::getInputLayout() const
{
	return inputLayout;
}