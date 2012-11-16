#include "fxManagement.h"

FXManagement::FXManagement()
{
	defaultVertexShader = nullptr;
	defaultPixelShader	= nullptr;
	inputLayout			= nullptr;

	defaultVS	= nullptr;
	defaultPS	= nullptr;
	error		= nullptr;
}

FXManagement::~FXManagement()
{
	if(defaultVertexShader)
		defaultVertexShader->Release();
	if(defaultPixelShader)
		defaultPixelShader->Release();
	if(inputLayout)
		inputLayout->Release();
	if(defaultVS)
		defaultVS->Release();
	if(defaultPS)
		defaultPS->Release();
	if(error)
		error->Release();
}

HRESULT FXManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initShaders(device);
	if(hr == S_OK)
		initInputLayout(device);

	return hr;
}

ID3D11VertexShader* FXManagement::getDefaultVertexShader() const
{
	return defaultVertexShader;
}

ID3D11PixelShader* FXManagement::getDefaultPixelShader() const
{
	return defaultPixelShader;
}

HRESULT FXManagement::initShaders(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initDefaultVertexShader(device);
	if(hr == S_OK)
		hr = initDefaultPixelShader(device);

	return hr;
}

HRESULT FXManagement::initDefaultVertexShader(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	
	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultVertexShader.cso", &defaultVS);
	if(hr == S_OK)
	{
		hr = device->CreateVertexShader(
			defaultVS->GetBufferPointer(),
			defaultVS->GetBufferSize(),
			nullptr,
			&defaultVertexShader);
	}

	return hr;
}

HRESULT FXManagement::initDefaultPixelShader(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(L"../../xkill-build/bin-Debug/defaultPixelShader.cso", &defaultPS);
	if(hr == S_OK)
	{
		hr = device->CreatePixelShader(
			defaultPS->GetBufferPointer(),
			defaultPS->GetBufferSize(),
			nullptr,
			&defaultPixelShader);
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

	hr = device->CreateInputLayout(ied, 3, defaultVS->GetBufferPointer(), defaultVS->GetBufferSize(), &inputLayout);

	return hr;
}