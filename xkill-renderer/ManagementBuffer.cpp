#include <cassert>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "Winfo.h"
#include "Buffer_SrvRtv.h"
#include "Buffer_SrvDsv.h"
#include "Buffer_SrvRtvUav.h"
#include "renderingUtilities.h"

#include "ManagementBuffer.h"

typedef DirectX::PackedVector::XMCOLOR XMCOLOR;

ManagementBuffer::ManagementBuffer(Winfo* winfo)
{
	winfo_ = winfo;
	getDownSampleDim(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		downSampleWidth_, 
		downSampleHeight_);

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;

	//Glow
	glowHigh_		= nullptr;
	glowLow_		= nullptr;
	glowLowUtil_	= nullptr;
	downSampleWidth_	= 1; //Set these to an arbitrary >0 value as resize() will be called before init. This requires a positive value.
	downSampleHeight_	= 1;
	ZeroMemory(&downSampleViewport_, sizeof(D3D11_VIEWPORT));

	//Shadowmap
	shadowMap_ = nullptr;
	ZeroMemory(&shadowViewport_, sizeof(D3D11_VIEWPORT));

	//SSAO
	ssaoWidth_	= 1; //Set these to an arbitrary >0 value as resize() will be called before init. This requires a positive value.
	ssaoHeight_ = 1;
	ssaoMap_ = nullptr;
	ZeroMemory(&ssaoViewport_, sizeof(D3D11_VIEWPORT));

	randomTex_ = nullptr;
	randomSRV_ = nullptr;
}
ManagementBuffer::~ManagementBuffer()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);

	SAFE_DELETE(glowHigh_);
	SAFE_DELETE(glowLow_);
	SAFE_DELETE(glowLowUtil_);

	SAFE_DELETE(shadowMap_);

	SAFE_DELETE(ssaoMap_);

	SAFE_RELEASE(randomTex_);
	SAFE_RELEASE(randomSRV_);
}

void ManagementBuffer::reset()
{
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_RESET(gBuffers_[i]);

	SAFE_RESET(glowHigh_);
	SAFE_RESET(glowLow_);
	SAFE_RESET(glowLowUtil_);

	SAFE_RESET(shadowMap_);

	SAFE_RESET(ssaoMap_);
}
HRESULT ManagementBuffer::resize(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	//Get new dimensions:
	getDownSampleDim(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		downSampleWidth_, 
		downSampleHeight_);
	getSSAODim(
		winfo_->getScreenWidth(),
		winfo_->getScreenHeight(),
		ssaoWidth_,
		ssaoHeight_);

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS && SUCCEEDED(hr); i++)
	{
		//G-Buffers always the same dimensions as screen.
		hr = gBuffers_[i]->resize(
			device, 
			winfo_->getScreenWidth(),
			winfo_->getScreenHeight());
	}

	if(SUCCEEDED(hr))
	{
		//High res glowmap always the same dimensions as screen.
		hr = glowHigh_->resize(
			device,
			winfo_->getScreenWidth(),
			winfo_->getScreenHeight());
	}
	if(SUCCEEDED(hr))
	{
		//Low res glowmap scaled.
		hr = glowLow_->resize(
			device,
			downSampleWidth_,
			downSampleHeight_);
	}
	if(SUCCEEDED(hr))
	{
		//Low res glowmap scaled.
		hr = glowLowUtil_->resize(
			device, 
			downSampleWidth_, 
			downSampleHeight_);
	}

	if(SUCCEEDED(hr))
	{
		hr = shadowMap_->resize(
			device, 
			SHADOWMAP_DIM, 
			SHADOWMAP_DIM);
	}

	if(SUCCEEDED(hr))
	{
		hr = ssaoMap_->resize(
			device,
			ssaoWidth_,
			ssaoHeight_);
	}

	//Also resize viewports:
	ZeroMemory(&downSampleViewport_, sizeof(D3D11_VIEWPORT));
	downSampleViewport_.TopLeftX	= 0;
	downSampleViewport_.TopLeftY	= 0;
	downSampleViewport_.Width		= static_cast<FLOAT>(downSampleWidth_);
	downSampleViewport_.Height		= static_cast<FLOAT>(downSampleHeight_);
	downSampleViewport_.MinDepth	= 0;
	downSampleViewport_.MaxDepth	= 1;

	ZeroMemory(&shadowViewport_, sizeof(D3D11_VIEWPORT));
	shadowViewport_.TopLeftX	= 0;
	shadowViewport_.TopLeftY	= 0;
	shadowViewport_.Width		= static_cast<FLOAT>(SHADOWMAP_DIM);
	shadowViewport_.Height		= static_cast<FLOAT>(SHADOWMAP_DIM);
	shadowViewport_.MinDepth	= 0.0f;
	shadowViewport_.MaxDepth	= 1.0f;

	ZeroMemory(&ssaoViewport_, sizeof(D3D11_VIEWPORT));
	ssaoViewport_.TopLeftX	= 0;
	ssaoViewport_.TopLeftY	= 0;
	ssaoViewport_.Width		= static_cast<FLOAT>(ssaoWidth_);
	ssaoViewport_.Height	= static_cast<FLOAT>(ssaoHeight_);
	ssaoViewport_.MinDepth	= 0.0f;
	ssaoViewport_.MaxDepth	= 1.0f;

	return hr;
}

HRESULT ManagementBuffer::init(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	HRESULT hr = S_OK;

	//Init g-buffers
	hr = initAlbedo(device);
	if(SUCCEEDED(hr))
		hr = initNormal(device);
	if(SUCCEEDED(hr))
		hr = initMaterial(device);

	if(SUCCEEDED(hr))
		hr = initGlow(device, devcon);

	if(SUCCEEDED(hr))
		hr = initShadow(device);

	if(SUCCEEDED(hr))
		hr = initSSAO(device);

	if(SUCCEEDED(hr))
		hr = initRandom(device);

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
		getFormat(BUFFER_FORMAT_ALBEDO),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT);
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
		getFormat(BUFFER_FORMAT_NORMAL),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT);
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
		getFormat(BUFFER_FORMAT_MATERIAL),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT);
	hr = gBuffer->init(device);
	
	gBuffers_[GBUFFERID_MATERIAL] = gBuffer;

	return hr;
}
HRESULT ManagementBuffer::initGlow(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	HRESULT hr = S_OK;

	//Init GlowBufHigh with given dimensions.
	glowHigh_ = new Buffer_SrvRtvUav(
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(),
		MULTISAMPLES_GBUFFERS, //?
		getFormat(BUFFER_FORMAT_GLOW_HIGH),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_USAGE_DEFAULT);
	hr = glowHigh_->init(device);

	//Init GlowBufLow with pre-computed dimensions.
	glowLow_ = new Buffer_SrvRtvUav(
		downSampleWidth_,
		downSampleHeight_,
		MULTISAMPLES_GBUFFERS, //?
		getFormat(BUFFER_FORMAT_GLOW_LOW),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_USAGE_DEFAULT);
	hr = glowLow_->init(device);

	glowLowUtil_ = new Buffer_SrvRtvUav(
		downSampleWidth_,
		downSampleHeight_,
		MULTISAMPLES_GBUFFERS, //?
		getFormat(BUFFER_FORMAT_GLOW_LOW),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_USAGE_DEFAULT);
	hr = glowLowUtil_->init(device);

	//Init downsampled glowmap viewport.
	ZeroMemory(&downSampleViewport_, sizeof(D3D11_VIEWPORT));
	downSampleViewport_.TopLeftX	= 0;
	downSampleViewport_.TopLeftY	= 0;
	downSampleViewport_.Width		= static_cast<FLOAT>(downSampleWidth_);
	downSampleViewport_.Height		= static_cast<FLOAT>(downSampleHeight_);
	downSampleViewport_.MinDepth	= 0;
	downSampleViewport_.MaxDepth	= 1;

	return hr;
}
HRESULT ManagementBuffer::initShadow(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	shadowMap_ = new Buffer_SrvDsv(
		SHADOWMAP_DIM,
		SHADOWMAP_DIM,
		1,
		DXGI_FORMAT_R24G8_TYPELESS,
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
		DXGI_FORMAT_D24_UNORM_S8_UINT);
	hr = shadowMap_->init(device);

	ZeroMemory(&shadowViewport_, sizeof(D3D11_VIEWPORT));
	shadowViewport_.TopLeftX	= 0;
	shadowViewport_.TopLeftY	= 0;
	shadowViewport_.Width		= static_cast<FLOAT>(SHADOWMAP_DIM);
	shadowViewport_.Height		= static_cast<FLOAT>(SHADOWMAP_DIM);
	shadowViewport_.MinDepth	= 0.0f;
	shadowViewport_.MaxDepth	= 1.0f;

	return hr;
}
HRESULT ManagementBuffer::initSSAO(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	ssaoMap_ = new Buffer_SrvRtvUav(
		ssaoWidth_,
		ssaoHeight_,
		1,
		getFormat(BUFFER_FORMAT_SSAO),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_USAGE_DEFAULT);

	ZeroMemory(&ssaoViewport_, sizeof(D3D11_VIEWPORT));
	ssaoViewport_.TopLeftX	= 0;
	ssaoViewport_.TopLeftY	= 0;
	ssaoViewport_.Width		= static_cast<FLOAT>(ssaoWidth_);
	ssaoViewport_.Height	= static_cast<FLOAT>(ssaoHeight_);
	ssaoViewport_.MinDepth	= 0.0f;
	ssaoViewport_.MaxDepth	= 1.0f;

	return hr;
}
HRESULT ManagementBuffer::initRandom(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= RANDOM_DIM;
	texDesc.Height				= RANDOM_DIM;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= getFormat(R8_G8_B8_A8__UNORM);
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = RANDOM_DIM * sizeof(XMCOLOR);

	XMCOLOR color[256 * 256];
	for(int i = 0; i < 256; ++i)
	{
		for(int j = 0; j < 256; ++j)
		{
			DirectX::XMFLOAT3 v(
				GET_RANDOM(), 
				GET_RANDOM(), 
				GET_RANDOM());

			color[i * 256 + j] = XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}
	initData.pSysMem = color;

	hr = device->CreateTexture2D(&texDesc, &initData, &randomTex_);
	if(SUCCEEDED(hr))
		hr = device->CreateShaderResourceView(randomTex_, NULL, &randomSRV_);

	return hr;
}

void ManagementBuffer::clearBuffers(ID3D11DeviceContext* devcon)
{
	//Clear all G-Buffers.
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO],		CLEARCOLOR_RED);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL],		CLEARCOLOR_BLUE);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_MATERIAL],	CLEARCOLOR_GREEN);

	//Clear glow-buffers:
	devcon->ClearRenderTargetView(glowHigh_->getRTV(), CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(glowLow_->getRTV(), CLEARCOLOR_BLACK);
	devcon->ClearRenderTargetView(glowLowUtil_->getRTV(), CLEARCOLOR_BLACK);

	//Clear shadowmap:
	ID3D11DepthStencilView* dsv = shadowMap_->getDSV();
	devcon->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Clear SSAO-map:
	ID3D11RenderTargetView* rtv = ssaoMap_->getRTV();
	devcon->ClearRenderTargetView(rtv, CLEARCOLOR_BLACK);
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
	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS + 3];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();

	resourceViews[GBUFFERID_NUM_BUFFERS]		= glowHigh_->getSRV();
	resourceViews[GBUFFERID_NUM_BUFFERS + 1]	= shadowMap_->getSRV();
	resourceViews[GBUFFERID_NUM_BUFFERS + 2]	= ssaoMap_->getSRV();

	devcon->CSSetShaderResources(
		0, 
		GBUFFERID_NUM_BUFFERS + 3,
		resourceViews);
}
void ManagementBuffer::unsetBuffersAsCSShaderResources(ID3D11DeviceContext* devcon)
{
	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS + 3];
	for(int i = 0; i < GBUFFERID_NUM_BUFFERS + 3; i++)
		resourceViews[i] = nullptr;

	devcon->CSSetShaderResources(
		0, 
		GBUFFERID_NUM_BUFFERS + 3, 
		resourceViews);
}

void ManagementBuffer::setBuffer(ID3D11DeviceContext* devcon, SET_ID setID, SET_TYPE setType, SET_STAGE setStage, unsigned int shaderRegister)
{
	Buffer_Srv* buffer = nullptr;
	switch (setID)
	{
	case SET_ID_GLOW_HIGH:
		buffer = glowHigh_;
		break;
	case SET_ID_GLOW_LOW:
		buffer = glowLow_;
		break;
	case SET_ID_GLOW_LOW_UTIL:
		buffer = glowLowUtil_;
		break;
	case SET_ID_SHADOW:
		buffer = shadowMap_;
		break;
	case SET_ID_NORMAL:
		buffer = gBuffers_[GBUFFERID_NORMAL];
		break;
	}

	ID3D11DepthStencilView* dsv = NULL; //for brevity
	switch (setType)
	{
	case SET_TYPE_RTV:
		{
			ID3D11RenderTargetView* renderTargets[1];
			renderTargets[0] = ((Buffer_SrvRtv*)buffer)->getRTV();

			devcon->OMSetRenderTargets(
				1,
				renderTargets,
				dsv); //shader reg irrelevant
			break;
		}
	case SET_TYPE_SRV:
		{
			ID3D11ShaderResourceView* resourceViews[1];
			resourceViews[0] = buffer->getSRV();

			switch(setStage)
			{
			case SET_STAGE_PS:
				devcon->PSSetShaderResources(
					shaderRegister, 
					1,
					resourceViews);
				break;
			case SET_STAGE_CS:
				devcon->CSSetShaderResources(
					shaderRegister, 
					1,
					resourceViews);
				break;
			}
			
			break;
		}
	case SET_TYPE_UAV:
		{
			ID3D11UnorderedAccessView* uav = ((Buffer_SrvRtvUav*)buffer)->getUAV();
			devcon->CSSetUnorderedAccessViews(
				shaderRegister, 
				1, 
				&uav, 
				nullptr);

			break;
		}
	case SET_TYPE_DSV:
		{
			dsv = shadowMap_->getDSV();
			ID3D11RenderTargetView* renderTargets[1] = { NULL };
			devcon->OMSetRenderTargets(
				1, //register and stage irrelevant
				renderTargets, 
				dsv); //set null rendertargets, as we want no colour-writes.
			break;
		}
	}
}
void ManagementBuffer::unset(
	ID3D11DeviceContext* devcon,
	SET_TYPE setType,
	SET_STAGE setStage,
	unsigned int shaderRegister)
{
	ID3D11DepthStencilView* dsv = NULL; //for brevity
	switch (setType)
	{
	case SET_TYPE_RTV:
		{
			ID3D11RenderTargetView* renderTargets[1];
			renderTargets[0] = nullptr;

			devcon->OMSetRenderTargets(
				1, 
				renderTargets, 
				dsv); //register irrelevant

			break;
		}
	case SET_TYPE_SRV:
		{
			ID3D11ShaderResourceView* resourceViews[1];
			resourceViews[0] = nullptr;

			switch (setStage)
			{
			case SET_STAGE_PS:
				devcon->PSSetShaderResources(
					shaderRegister, 
					1,
					resourceViews);
				break;
			case SET_STAGE_CS:
				devcon->CSSetShaderResources(
					shaderRegister, 
					1,
					resourceViews);
				break;
			}

			break;
		}
	case SET_TYPE_UAV:
		{
			ID3D11UnorderedAccessView* uavs[] = { nullptr };
			devcon->CSSetUnorderedAccessViews(
				shaderRegister, 
				1, 
				uavs, 
				nullptr);

			break;
		}
	case SET_TYPE_DSV:
		{
			ID3D11RenderTargetView* renderTargets[1] = { NULL };
			devcon->OMSetRenderTargets(1, renderTargets, NULL);
			break;
		}
	}
}

DXGI_FORMAT ManagementBuffer::getFormat(BUFFER_FORMAT format)
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
	case R16__FLOAT:
		dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
		break;
	}

	return dxgiFormat;
}
void ManagementBuffer::getDownSampleDim(
	unsigned int screenWidth,
	unsigned int screenHeight,
	unsigned int& downSampleWidth, 
	unsigned int& downSampleHeight)
{
	downSampleWidth		= screenWidth	/ DOWNSAMPLE_SCREEN_RES_FACTOR;
	downSampleHeight	= screenHeight	/ DOWNSAMPLE_SCREEN_RES_FACTOR;
}
void ManagementBuffer::getSSAODim(
	unsigned int screenWidth,
	unsigned int screenHeight,
	unsigned int& ssaoWidth,
	unsigned int& ssaoHeight)
{
	ssaoWidth	= screenWidth	/ SSAO_MAP_SCREEN_RES_FACTOR;
	ssaoHeight	= screenHeight	/ SSAO_MAP_SCREEN_RES_FACTOR;
}

D3D11_VIEWPORT ManagementBuffer::getDownSampledViewport()
{
	return downSampleViewport_;
}
D3D11_VIEWPORT ManagementBuffer::getShadowViewport()
{
	return shadowViewport_;
}