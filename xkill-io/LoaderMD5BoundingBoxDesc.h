#ifndef XKILL_IO_LOADER_MD5_BOUNDING_BOX_DESC_H
#define XKILL_IO_LOADER_MD5_BOUNDING_BOX_DESC_H

#include <DirectXMath.h>

struct LoaderMD5BoundingBoxDesc
{
	DirectX::XMFLOAT3 min_;
	DirectX::XMFLOAT3 max_;

	LoaderMD5BoundingBoxDesc()
	{
		min_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		max_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	LoaderMD5BoundingBoxDesc(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max)
	{
		min_ = min;
		max_ = max;
	}
};

#endif //XKILL_IO_LOADER_MD5_BOUNDING_BOX_DESC_H