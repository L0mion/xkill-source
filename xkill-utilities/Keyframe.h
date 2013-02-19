#ifndef XKILL_UTILITIES_KEYFRAME_H
#define XKILL_UTILITIES_KEYFRAME_H

#include <cstring>

#include <DirectXMath.h>

#include "dllUtilities.h"

struct DLL_U Keyframe
{
	Keyframe()
	{
		timePosition		= 0;
		translation			= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale				= DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		rotationQuaternion	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	}

	float				timePosition;
	DirectX::XMFLOAT3	translation;
	DirectX::XMFLOAT3	scale;
	DirectX::XMFLOAT4	rotationQuaternion;
};

#endif //XKILL_UTILITIES_KEYFRAME_H