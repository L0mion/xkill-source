#include <CBPerFrame.h>

#include <constantBufferDesc.h>

CBPerFrame::CBPerFrame()
{
}

CBPerFrame::~CBPerFrame()
{
}

void CBPerFrame::reset()
{
	CB::reset();
}

void CBPerFrame::update(ID3D11DeviceContext* devcon, MatF4 worldViewProj)
{
	CBPerFrameDesc cbDesc;
	cbDesc.worldViewProj_ = worldViewProj;

	devcon->UpdateSubresource(constantBuffer_, 0, 0, &cbDesc, 0, 0);
}