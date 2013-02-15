#include <cassert>

#include "Winfo.h"
#include "Buffer_SrvRtv.h"
#include "Buffer_SrvRtvUav.h"
#include "renderingUtilities.h"

#include "ManagementBuffer.h"

DXGI_FORMAT getFormat(GBUFFER_FORMAT format)
{
	DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

	switch(format)
	{
	case R8_G8_B8_A8__UNORM:
		dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case R16_G16_B16_A16__FLOAT:
		dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	case R32_G32_B32_A32__FLOAT:
		dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	}

	return dxgiFormat;
}

ManagementBuffer::ManagementBuffer(Winfo* winfo)
{
	winfo_ = winfo;

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;

	glowHigh_	= nullptr;
	glowLow_	= nullptr;
}
ManagementBuffer::~ManagementBuffer()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);

	SAFE_DELETE(glowHigh_);
	SAFE_DELETE(glowLow_);
}

void ManagementBuffer::reset()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_RESET(gBuffers_[i]);

	SAFE_RESET(glowHigh_);
	SAFE_RESET(glowLow_);
}
HRESULT ManagementBuffer::resize(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS && SUCCEEDED(hr); i++)
	{
		hr = gBuffers_[i]->resize(
			device, 
			winfo_->getScreenWidth(),
			winfo_->getScreenHeight());
	}

	if(SUCCEEDED(hr))
	{
		hr = glowHigh_->resize(
			device,
			winfo_->getScreenWidth(),
			winfo_->getScreenHeight());
	}
	if(SUCCEEDED(hr))
	{
		unsigned int width	= winfo_->getScreenWidth()	/ 2;
		unsigned int height = winfo_->getScreenHeight()	/ 2;

		hr = glowLow_->resize(
			device,
			width,
			height);
	}

	return hr;
}

HRESULT ManagementBuffer::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	//Init g-buffers
	hr = initAlbedo(device);
	if(SUCCEEDED(hr))
		hr = initNormal(device);
	if(SUCCEEDED(hr))
		hr = initMaterial(device);

	if(SUCCEEDED(hr))
		hr = initGlow(device);

	return hr;
}
HRESULT ManagementBuffer::initAlbedo(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	Buffer_SrvRtv* gBuffer = nullptr;
	gBuffer = new Buffer_SrvRtv(
		winfo_->getScreenWidth(),
		winfo_->getScreenHeight(),
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_ALBEDO),
		false);
	hr = gBuffer->init(device);

	gBuffers_[GBUFFERID_ALBEDO] = gBuffer;

	return hr;
}
HRESULT ManagementBuffer::initNormal(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	Buffer_SrvRtv* gBuffer = nullptr;
	gBuffer = new Buffer_SrvRtv(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_NORMAL),
		false);
	hr = gBuffer->init(device);

	gBuffers_[GBUFFERID_NORMAL] = gBuffer;

	return hr;
}
HRESULT ManagementBuffer::initMaterial(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	Buffer_SrvRtv* gBuffer = nullptr;
	gBuffer = new Buffer_SrvRtv(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_MATERIAL),
		false);
	hr = gBuffer->init(device);
	
	gBuffers_[GBUFFERID_MATERIAL] = gBuffer;

	return hr;
}
HRESULT ManagementBuffer::initGlow(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	//Init GlowBufHigh
	glowHigh_ = new Buffer_SrvRtvUav(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(),
		MULTISAMPLES_GBUFFERS, //?
		getFormat(GBUFFER_FORMAT_GLOW_HIGH),
		false);
	hr = glowHigh_->init(device);

	//Init GlowBufLow
	unsigned int width	= winfo_->getScreenWidth()  / 2;
	unsigned int height	= winfo_->getScreenHeight() / 2;
	glowLow_ = new Buffer_SrvRtvUav(
		width,
		height,
		MULTISAMPLES_GBUFFERS, //?
		getFormat(GBUFFER_FORMAT_GLOW_LOW),
		false);
	hr = glowLow_->init(device);

	return hr;
}

void ManagementBuffer::clearBuffers(ID3D11DeviceContext* devcon)
{
	//Clear all G-Buffers.
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO],		CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL],		CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_MATERIAL],	CLEARCOLOR_BLACK);

	//Clear glow-buffers.
	devcon->ClearRenderTargetView(glowHigh_->getRTV(), CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(glowLow_->getRTV(), CLEARCOLOR_BLACK);
}
void ManagementBuffer::setBuffersAndDepthBufferAsRenderTargets(
	ID3D11DeviceContext*	devcon, 
	ID3D11DepthStencilView*	depthBuffer)
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS + 1];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();

	renderTargets[GBUFFERID_NUM_BUFFERS] = glowHigh_->getRTV();

	devcon->OMSetRenderTargets(
		GBUFFERID_NUM_BUFFERS + 1,
		renderTargets, 
		depthBuffer);
}
void ManagementBuffer::unsetBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext* devcon)
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS + 1];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS + 1; i++)
		renderTargets[i] = nullptr;

	devcon->OMSetRenderTargets(
		GBUFFERID_NUM_BUFFERS + 1, 
		renderTargets, 
		NULL);
}
void ManagementBuffer::setBuffersAsCSShaderResources(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS + 1];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();

	resourceViews[GBUFFERID_NUM_BUFFERS] = glowLow_->getSRV(); //

	devcon->CSSetShaderResources(
		0, 
		GBUFFERID_NUM_BUFFERS + 1,
		resourceViews);
}
void ManagementBuffer::unsetBuffersAsCSShaderResources(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS + 1];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS + 1; i++)
		resourceViews[i] = nullptr;

	devcon->CSSetShaderResources(
		0, 
		GBUFFERID_NUM_BUFFERS + 1, 
		resourceViews);
}

void ManagementBuffer::setGlowLowAsRTV(ID3D11DeviceContext* devcon)
{
	ID3D11RenderTargetView* renderTargets[1];
	renderTargets[0] = glowLow_->getRTV();

	devcon->OMSetRenderTargets(
		1,
		renderTargets,
		NULL);
}
void ManagementBuffer::unsetGlowLowAsRTV(ID3D11DeviceContext* devcon)
{
	ID3D11RenderTargetView* renderTargets[1];
	renderTargets[0] = nullptr;

	devcon->OMSetRenderTargets(
		1, 
		renderTargets, 
		NULL);
}

void ManagementBuffer::setGlowHighAsSRV(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* srv = glowHigh_->getSRV();

	devcon->PSSetShaderResources(
		SHADER_REGISTER_GLOW, 
		1, 
		&srv);
}
void ManagementBuffer::unsetGlowHighAsSrv(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* nullViews[1] = { nullptr };

	devcon->PSSetShaderResources(
		SHADER_REGISTER_GLOW,
		1,
		nullViews);
}