#include "Winfo.h"
#include "gBuffer.h"
#include "renderingUtilities.h"

#include "ManagementGBuffer.h"

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

ManagementGBuffer::ManagementGBuffer(Winfo* winfo)
{
	winfo_ = winfo;

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;
}
ManagementGBuffer::~ManagementGBuffer()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);
}

void ManagementGBuffer::reset()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_RESET(gBuffers_[i]);
}
HRESULT ManagementGBuffer::resize(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS && SUCCEEDED(hr); i++)
	{
		hr = gBuffers_[i]->resize(
			device, 
			winfo_->getScreenWidth(), 
			winfo_->getScreenHeight());
	}

	return hr;
}

HRESULT ManagementGBuffer::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initAlbedo(device);
	if(SUCCEEDED(hr))
		hr = initNormal(device);
	if(SUCCEEDED(hr))
		hr = initMaterial(device);

	return hr;
}
HRESULT ManagementGBuffer::initAlbedo(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;
	gBuffer = new GBuffer(
		winfo_->getScreenWidth(),
		winfo_->getScreenHeight(),
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_ALBEDO));
	hr = gBuffer->init(device);

	gBuffers_[GBUFFERID_ALBEDO] = gBuffer;

	return hr;
}
HRESULT ManagementGBuffer::initNormal(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;
	gBuffer = new GBuffer(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_NORMAL));
	hr = gBuffer->init(device);

	gBuffers_[GBUFFERID_NORMAL] = gBuffer;

	return hr;
}
HRESULT ManagementGBuffer::initMaterial(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;
	gBuffer = new GBuffer(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		MULTISAMPLES_GBUFFERS, 
		getFormat(GBUFFER_FORMAT_MATERIAL));
	hr = gBuffer->init(device);
	
	gBuffers_[GBUFFERID_MATERIAL] = gBuffer;

	return hr;
}

void ManagementGBuffer::clearGBuffers(ID3D11DeviceContext* devcon)
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	
	//clear all gbuffers in black
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO],		CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL],		CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_MATERIAL],	CLEARCOLOR_BLACK);
}
void ManagementGBuffer::setGBuffersAndDepthBuffer(
	ID3D11DeviceContext*	devcon, 
	ID3D11DepthStencilView*	depthBuffer)
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();

	devcon->OMSetRenderTargets(
		GBUFFERID_NUM_BUFFERS,
		renderTargets, 
		depthBuffer);
}
void ManagementGBuffer::unsetGBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext* devcon)
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();

	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = nullptr;

	devcon->OMSetRenderTargets(
		GBUFFERID_NUM_BUFFERS, 
		renderTargets, 
		NULL);
}
void ManagementGBuffer::setGBuffersAsCSShaderResources(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();

	devcon->CSSetShaderResources(
		0, 
		GBUFFERID_NUM_BUFFERS, 
		resourceViews);
}