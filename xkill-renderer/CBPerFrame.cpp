#include "CBPerFrame.h"

#include "constantBufferDesc.h"

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

void CBPerFrame::update(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT4X4 worldViewProj,
						DirectX::XMFLOAT4X4 view,
						DirectX::XMFLOAT4X4 viewInverse,
						DirectX::XMFLOAT4X4 projection,
						DirectX::XMFLOAT3	eyePosition)
{
	CBPerFrameDesc cbDesc;
	cbDesc.worldViewProj_	= worldViewProj;
	cbDesc.view_			= view;
	cbDesc.viewInverse_		= viewInverse;
	cbDesc.projection_		= projection;
	cbDesc.eyePosition_		= eyePosition;

	devcon->UpdateSubresource(constantBuffer_, 0, 0, &cbDesc, 0, 0);
}