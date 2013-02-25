#ifndef XKILL_RENDERER_SHADOWMAPPING_H
#define XKILL_RENDERER_SHADOWMAPPING_H

#include <DirectXMath.h>
struct SceneBounds
{
	DirectX::XMFLOAT3 center;
	float radius;

	SceneBounds() { center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); radius = 0.0f; }
};

struct ShadowMatrices
{
	DirectX::XMFLOAT4X4 view_;
	DirectX::XMFLOAT4X4 proj_;
	DirectX::XMFLOAT4X4 shadowMapTransform_;
};

#endif //XKILL_RENDERER_SHADOWMAPPING_H