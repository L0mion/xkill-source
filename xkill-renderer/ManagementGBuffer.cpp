#include "Winfo.h"
#include "gBuffer.h"
#include "renderingUtilities.h"

#include "ManagementGBuffer.h"

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
	if(hr)
		hr = initNormal(device);
	if(hr)
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
		DXGI_FORMAT_R32G32B32A32_FLOAT);
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
		DXGI_FORMAT_R32G32B32A32_FLOAT);
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
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	hr = gBuffer->init(device);
	
	gBuffers_[GBUFFERID_MATERIAL] = gBuffer;

	return hr;
}