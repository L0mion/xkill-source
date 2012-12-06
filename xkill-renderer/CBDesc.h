#ifndef XKILL_RENDERER_CBDESC_H
#define XKILL_RENDERER_CBDESC_H

#include <DirectXMath.h>

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBFrameDesc
{
	DirectX::XMFLOAT4X4 viewMatrix_;
	DirectX::XMFLOAT4X4 viewMatrixInverse_;
	DirectX::XMFLOAT4X4 projectionMatrix_;
	DirectX::XMFLOAT4X4 projectionMatrixInverse_;
	DirectX::XMFLOAT3	eyePosition_;
	unsigned int		numLights_;
};
static const unsigned int CB_FRAME_DESC_SIZE = 272;

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBInstanceDesc
{
	unsigned int screenWidth_;
	unsigned int screenHeight_;
	unsigned int pad1_;
	unsigned int pad2_;
};
static const unsigned int CB_INSTANCE_DESC_SIZE = 16;

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBObjectDesc
{
	DirectX::XMFLOAT4X4 finalMatrix_;
	DirectX::XMFLOAT4X4 worldMatrix_;
	DirectX::XMFLOAT4X4 worldMatrixInverse_;
};
static const unsigned int CB_OBJECT_DESC_SIZE = 192;


#endif //XKILL_RENDERER_CBDESC_H