#ifndef XKILL_IO_LOADER_MD5_BASE_FRAME_DESC_H
#define XKILL_IO_LOADER_MD5_BASE_FRAME_DESC_H

#include <DirectXMath.h>

struct LoaderMD5BaseFrameDesc
{
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT4 orientationQuaternion_;

	LoaderMD5BaseFrameDesc()
	{
		position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		orientationQuaternion_ = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	LoaderMD5BaseFrameDesc(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 orientationQuaternion)
	{
		position_ = position;
		orientationQuaternion_ = orientationQuaternion;
	}
};

#endif //XKILL_IO_LOADER_MD5_BASE_FRAME_DESC_H