#include "fxManagement.h"
#include "renderingUtilities.h"

FXManagement::FXManagement()
{
	ilManagement = nullptr;

	defaultVS_				= nullptr;
	defaultPS_				= nullptr;
	defaultDeferredVS_		= nullptr;
	defaultDeferredPS_		= nullptr;
	defaultCS_				= nullptr;
	ilDefaultVSPosNormTex_	= nullptr;
}

FXManagement::~FXManagement()
{
	//SAFE_DELETE(ilManagement);
	int debug = sizeof((*ilManagement));

	delete ilManagement;

	SAFE_DELETE(defaultVS_);
	SAFE_DELETE(defaultPS_);
	SAFE_DELETE(defaultDeferredVS_);
	SAFE_DELETE(defaultDeferredPS_);
	SAFE_DELETE(defaultCS_);
	
	SAFE_RELEASE(ilDefaultVSPosNormTex_);
}

void FXManagement::reset()
{
	defaultVS_->reset();
	defaultPS_->reset();
	defaultDeferredVS_->reset();
	defaultDeferredPS_->reset();
	defaultCS_->reset();
	
	SAFE_RELEASE(ilDefaultVSPosNormTex_);
}

HRESULT FXManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	initILManagement();

	hr = initShaders(device);
	if(SUCCEEDED(hr))
		hr = initILs(device);

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

	defaultVS_ = new ShaderVS();
	hr = defaultVS_->init(device, L"../../xkill-build/bin-Debug/defaultVS.cso");

	return hr;
}
HRESULT FXManagement::initDefaultPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultPS_ = new ShaderPS();
	hr = defaultPS_->init(device, L"../../xkill-build/bin-Debug/defaultPS.cso");

	return hr;
}
HRESULT FXManagement::initDefaultDeferredVS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultDeferredVS_ = new ShaderVS();
	hr = defaultDeferredVS_->init(device, L"../../xkill-build/bin-Debug/defaultDeferredVS.cso");

	return hr;
}
HRESULT FXManagement::initDefaultDeferredPS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultDeferredPS_ = new ShaderPS();
	hr = defaultDeferredPS_->init(device, L"../../xkill-build/bin-Debug/defaultDeferredPS.cso");

	return hr;
}
HRESULT FXManagement::initDefaultCS(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	defaultCS_ = new ShaderCS();
	hr = defaultCS_->init(device, L"../../xkill-build/bin-Debug/defaultCS.cso");

	return hr;
}

HRESULT FXManagement::initILs(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	initILManagement();

//	hr = initILDefaultVSPosNormTex(device);
	
	return hr;
}
void FXManagement::initILManagement()
{
	ilManagement = new IEDManagement();
	ilManagement->init();
}
HRESULT FXManagement::initILDefaultVSPosNormTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	unsigned int iedPosNormTexNumElements	= ilManagement->getIEDPosNormTexNumElements();
	D3D11_INPUT_ELEMENT_DESC* iedPosNormTex = ilManagement->getIEDPosNormTex();

	int debug = sizeof(D3D11_INPUT_ELEMENT_DESC);

	hr = device->CreateInputLayout(
		iedPosNormTex, 
		iedPosNormTexNumElements, 
		defaultVS_->getBlob()->GetBufferPointer(), 
		defaultVS_->getBlob()->GetBufferSize(), 
		&ilDefaultVSPosNormTex_);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initILDefaultVSPosNormTex CreateInputLayout failed");

	return hr;
}

ShaderVS* FXManagement::getDefaultVS() const
{
	return defaultVS_;
}
ShaderPS* FXManagement::getDefaultPS() const
{
	return defaultPS_;
}
ShaderVS* FXManagement::getDefaultDeferredVS()	const
{
	return defaultDeferredVS_;
}
ShaderPS* FXManagement::getDefaultDeferredPS() const
{
	return defaultDeferredPS_;
}
ShaderCS* FXManagement::getDefaultCS() const
{
	return defaultCS_;
}

ID3D11InputLayout* FXManagement::getILDefaultVSPosNormTex() const
{
	return ilDefaultVSPosNormTex_;
}