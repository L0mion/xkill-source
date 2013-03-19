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
	unsigned int numLightsDir;
	bool ssaoOn;
	bool shadowsOn;
	bool glowOn;
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
	float				zNear;
	float				zFar;
	float				viewportWidth;
	
	float viewportHeight;
	unsigned int numLightsPoint;
	unsigned int pad3;
	unsigned int pad4;
};
static const unsigned int CB_CAMERA_DESC_SIZE = 304;

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


static const unsigned int CB_BONE_DESC_NUM_BONES = 96;
//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBBoneDesc
{
	DirectX::XMFLOAT4X4 boneTransforms[CB_BONE_DESC_NUM_BONES];
};
static const unsigned int CB_BONE_DESC_SIZE = 6144;


//! Struct describing a constant buffer.
/*!
\ingroup xkill-renderer
*/
struct CBSpriteDesc
{
	DirectX::XMFLOAT4X4 spriteMatrix_;
};
static const unsigned int CB_SPRITE_DESC_SIZE = 64;

static const unsigned int NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED = 3;
struct CBBlurDesc
{
	DirectX::XMFLOAT4 blurKernelCompressed[NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED];
};
static const unsigned int CB_BLUR_DESC_SIZE = 48;

struct CBSSAODesc
{
	unsigned int ssaoWidth;
	unsigned int ssaoHeight;
	float occlusionRadius;
	float occlusionScale;

	float occlusionBias;
	float occlusionIntensity;
	unsigned int pad7;
	unsigned int pad8;
};
static const unsigned int CB_SSAO_DESC_SIZE = 32;

struct CBShadowDesc
{
	DirectX::XMFLOAT4X4 shadowMapTransform_;
};
static const unsigned int CB_SHADOW_DESC_SIZE = 64;

struct CBCharDesc
{
	DirectX::XMFLOAT3 glowmod;
	unsigned int pad9;
};
static const unsigned int CB_CHAR_DESC_SIZE = 16;

#endif //XKILL_RENDERER_CBDESC_H