#include "fxManagement.h"

FXManagement::FXManagement()
{
	defaultVS			= nullptr;
	defaultPS			= nullptr;
	defaultDeferredPS	= nullptr;
	inputLayout			= nullptr;

	blobDefaultVS			= nullptr;
	blobDefaultPS			= nullptr;
	blobDefaultDeferredPS	= nullptr;
	blobError				= nullptr;
}

FXManagement::~FXManagement()
{
	if(defaultVS)
		defaultVS->Release();
	if(defaultPS)
		defaultPS->Release();
	if(inputLayout)
		inputLayout->Release();
	if(blobDefaultVS)
		blobDefaultVS->Release();
	if(blobDefaultPS)
		blobDefaultPS->Release();
	if(blobDefaultDeferredPS)
		blobDefaultDeferredPS->Release();
	if(blobError)
		blobError->Release();
}

HRESULT FXManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initShaders(device);
	if(hr == S_OK)
		initInputLayout(device);

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

ID3D11PixelShader* FXManagement::getDefaultDeferredPS() const
{
	return defaultDeferredPS;
}

HRESULT FXManagement::initShaders(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initDefaultVS(device);
	if(hr == S_OK)
		hr = initDefaultPS(device);
	if(hr == S_OK)
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
	
	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultVertexShader.cso", &blobDefaultVS);
	if(hr == S_OK)
	{
		hr = device->CreateVertexShader(
			blobDefaultVS->GetBufferPointer(),
			blobDefaultVS->GetBufferSize(),
			nullptr,
			&defaultVS);
	}

	return hr;
}

HRESULT FXManagement::initDefaultPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultPixelShader.cso", &blobDefaultPS);
	if(hr == S_OK)
	{
		hr = device->CreatePixelShader(
			blobDefaultPS->GetBufferPointer(),
			blobDefaultPS->GetBufferSize(),
			nullptr,
			&defaultPS);
	}

	return hr;
}

HRESULT FXManagement::initDefaultDeferredPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultDeferredPS.cso", &blobDefaultDeferredPS);
	if(hr == S_OK)
	{
		hr = device->CreatePixelShader(
			blobDefaultDeferredPS->GetBufferPointer(),
			blobDefaultDeferredPS->GetBufferSize(),
			nullptr,
			&defaultDeferredPS);
	}

	return hr;
}

HRESULT FXManagement::initInputLayout(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = device->CreateInputLayout(ied, 3, blobDefaultVS->GetBufferPointer(), blobDefaultVS->GetBufferSize(), &inputLayout);

	return hr;
}