#ifndef XKILL_RENDERER_KEYFRAME_H
#define XKILL_RENDERER_KEYFRAME_H

#include <cstring>

#include <DirectXMath.h>

struct Keyframe
{
	Keyframe()
	{
		timePosition		= 0;
		translation			= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale				= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		rotationQuaternion	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	}

	float				timePosition;
	DirectX::XMFLOAT3	translation;
	DirectX::XMFLOAT3	scale;
	DirectX::XMFLOAT4	rotationQuaternion;
};

#endif //XKILL_RENDERER_KEYFRAME_H