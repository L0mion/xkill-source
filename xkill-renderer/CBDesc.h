#ifndef XKILL_RENDERER_CBDESC_H
#define XKILL_RENDERER_CBDESC_H

#include <DirectXMath.h>

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBInstanceDesc
{
	unsigned int screenWidth_;	//4 bytes
	unsigned int screenHeight_;	//4 bytes
	unsigned int pad1;			//4 bytes
	unsigned int pad2;			//4 bytes
};
static const unsigned int CB_INSTANCE_DESC_SIZE = 16;

//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBFrameDesc
{
	unsigned int numLights_;
	unsigned int pad1_;
	unsigned int pad2_;
	unsigned int pad3_;
};
static const unsigned int CB_FRAME_DESC_SIZE = 16;

struct CBCameraDesc
{
	DirectX::XMFLOAT4X4 viewMatrix_;
	DirectX::XMFLOAT4X4 viewMatrixInverse_;
	DirectX::XMFLOAT4X4 projectionMatrix_;
	DirectX::XMFLOAT4X4 projectionMatrixInverse_;
	DirectX::XMFLOAT3	eyePosition_;
	unsigned int		viewportTopX_;
	unsigned int		viewportTopY_;
};
static const unsigned int CB_CAMERA_DESC_SIZE = 288;

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

//! Struct describing a constant buffer being updated several times per object.
/*!
\ingroup xkill-renderer
*/
struct CBSubsetDesc
{
	DirectX::XMFLOAT3	specularTerm_;
	float				specularPower_;
};
static const unsigned int CB_SUBSET_DESC_SIZE = 16;


#endif //XKILL_RENDERER_CBDESC_H