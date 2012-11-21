#include "fxManagement.h"
#include "renderingUtilities.h"


FXManagement::FXManagement()
{
	defaultVS			= nullptr;
	defaultPS			= nullptr;
	defaultDeferredVS	= nullptr;
	defaultDeferredPS	= nullptr;
	defaultCS			= nullptr;
	inputLayout			= nullptr;
}

FXManagement::~FXManagement()
{
	SAFE_DELETE(defaultVS);
	SAFE_DELETE(defaultPS);
	SAFE_DELETE(defaultDeferredVS);
	SAFE_DELETE(defaultDeferredPS);
	SAFE_DELETE(defaultCS);
	
	SAFE_RELEASE(inputLayout);
}

void FXManagement::reset()
{
	defaultVS->reset();
	defaultPS->reset();
	defaultDeferredVS->reset();
	defaultDeferredPS->reset();
	defaultCS->reset();
	
	SAFE_RELEASE(inputLayout);
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
	if(SUCCEEDED(hr))
		hr = initDefaultCS(device);
	
	return hr;
}

HRESULT FXManagement::initDefaultVS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultVS = new ShaderVS();
	hr = defaultVS->init(device, L"../../xkill-build/bin-Debug/defaultVS.cso");

	return hr;
}

HRESULT FXManagement::initDefaultPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultPS = new ShaderPS();
	hr = defaultPS->init(device, L"../../xkill-build/bin-Debug/defaultPS.cso");

	return hr;
}

HRESULT FXManagement::initDefaultDeferredVS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultDeferredVS = new ShaderVS();
	hr = defaultDeferredVS->init(device, L"../../xkill-build/bin-Debug/defaultDeferredVS.cso");

	return hr;
}

HRESULT FXManagement::initDefaultDeferredPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultDeferredPS = new ShaderPS();
	hr = defaultDeferredPS->init(device, L"../../xkill-build/bin-Debug/defaultDeferredPS.cso");

	return hr;
}

HRESULT FXManagement::initDefaultCS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultCS = new ShaderCS();
	hr = defaultCS->init(device, L"../../xkill-build/bin-Debug/defaultCS.cso");

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
		defaultVS->getBlob()->GetBufferPointer(), 
		defaultVS->getBlob()->GetBufferSize(), 
		&inputLayout);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initInputLayout CreateInputLayout failed");
	return hr;
}

ShaderVS* FXManagement::getDefaultVS() const
{
	return defaultVS;
}

ShaderPS* FXManagement::getDefaultPS() const
{
	return defaultPS;
}

ShaderVS* FXManagement::getDefaultDeferredVS()	const
{
	return defaultDeferredVS;
}

ShaderPS* FXManagement::getDefaultDeferredPS() const
{
	return defaultDeferredPS;
}

ShaderCS* FXManagement::getDefaultCS() const
{
	return defaultCS;
}

ID3D11InputLayout* FXManagement::getInputLayout() const
{
	return inputLayout;
}