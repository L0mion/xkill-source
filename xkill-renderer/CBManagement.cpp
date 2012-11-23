
#include "CBManagement.h"

#include "ConstantBufferDesc.h"
#include "RenderingUtilities.h"

CBManagement::CBManagement()
{
	cbPerFrame_		= nullptr;
	cbPerInstance_	= nullptr;
}

CBManagement::~CBManagement()
{
	SAFE_DELETE(cbPerFrame_);
	SAFE_DELETE(cbPerInstance_);
}

void CBManagement::reset()
{
	cbPerFrame_->reset();
	cbPerInstance_->reset();
}

HRESULT CBManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initCBPerFrame(device);
	if(SUCCEEDED(hr))
		hr = initCBPerInstance(device);

	return hr;
}

HRESULT CBManagement::initCBPerFrame(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	cbPerFrame_ = new CBPerFrame();
	hr = cbPerFrame_->init(device, cbPerFrameSize);
	return hr;
}

HRESULT CBManagement::initCBPerInstance(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	cbPerInstance_ = new CBPerInstance();
	hr = cbPerInstance_->init(device, cbPerInstanceSize);
	return hr;
}

CBPerFrame* CBManagement::getCBPerFrame() const
{
	return cbPerFrame_;
}

CBPerInstance* CBManagement::getCBPerInstance() const
{
	return cbPerInstance_;
}