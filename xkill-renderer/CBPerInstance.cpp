#include "CBPerInstance.h"

#include "constantBufferDesc.h"

CBPerInstance::CBPerInstance()
{
}

CBPerInstance::~CBPerInstance()
{
}

void CBPerInstance::reset()
{
	CB::reset();
}

void CBPerInstance::update(ID3D11DeviceContext* devcon,
						   unsigned int screenWidth,
						   unsigned int screenHeight,
						   unsigned int tileWidth,
						   unsigned int tileHeight)
{
	CBPerInstanceDesc cbDesc;
	cbDesc.screenWidth_		= screenWidth;
	cbDesc.screenHeight_	= screenHeight;
	cbDesc.tileWidth_		= tileWidth;
	cbDesc.tileHeight_		= tileHeight;

	devcon->UpdateSubresource(constantBuffer_, 0, 0, &cbDesc, 0, 0);
}