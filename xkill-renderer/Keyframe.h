#ifndef XKILL_RENDERER_KEYFRAME_H
#define XKILL_RENDERER_KEYFRAME_H

#include <cstring>

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
};

struct Keyframe
{
	Keyframe()
	{
		memset(this, 0, sizeof(Keyframe));
	}

	float				timePosition;
	DirectX::XMFLOAT3	translation;
	DirectX::XMFLOAT3	scale;
	DirectX::XMFLOAT4	rotationQuaternion;
};

#endif //XKILL_RENDERER_KEYFRAME_H