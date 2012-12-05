#ifndef XKILL_RENDERER_CBDESC_H
#define XKILL_RENDERER_CBDESC_H

#include <DirectXMath.h>

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBFrameDesc
{
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 viewMatrixInverse;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4X4 projectionMatrixInverse;
	DirectX::XMFLOAT3	eyePosition;
	unsigned int		numLights;
};
static const unsigned int CB_FRAME_DESC_SIZE = 336;

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBObjectDesc
{
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 worldMatrixInverse;
};
static const unsigned int CB_OBJECT_DESC_SIZE = 192;

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBInstanceDesc
{
	unsigned int screenWidth;
	unsigned int screenHeight;
	unsigned int pad1;
	unsigned int pad2;
};
static const unsigned int CB_INSTANCE_DESC_SIZE = 16;

#endif //XKILL_RENDERER_CBDESC_H