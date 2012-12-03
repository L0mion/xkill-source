#ifndef XKILL_RENDERER_CBDESC_H
#define XKILL_RENDERER_CBDESC_H

#include <DirectXMath.h>

struct CBFrameDesc
{
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 viewMatrixInverse;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4X4 projectionMatrixInverse;
	DirectX::XMFLOAT3	eyePosition;
	int					pad1;
};
static const unsigned int CB_FRAME_DESC_SIZE = 336;

struct CBInstanceDesc
{
	unsigned int screenWidth;
	unsigned int screenHeight;
	unsigned int pad1;
	unsigned int pad2;
};
static const unsigned int CB_INSTANCE_DESC_SIZE = 16;

#endif //XKILL_RENDERER_CBDESC_H