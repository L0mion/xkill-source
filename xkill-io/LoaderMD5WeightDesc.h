#ifndef XKILL_IO_LOADER_MD5_WEIGHT_DESC_H
#define XKILL_IO_LOADER_MD5_WEIGHT_DESC_H

#include <DirectXMath.h>

struct LoaderMD5WeightDesc
{
	int					jointID_;
	float				bias_;
	DirectX::XMFLOAT3	position_;

	LoaderMD5WeightDesc()
	{
		jointID_  = -1;
		bias_	  = 0.0f;
		position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	LoaderMD5WeightDesc(int jointID, float bias, DirectX::XMFLOAT3 position)
	{
		jointID_	= jointID;
		bias_		= bias;
		position_	= position;
	}
};

#endif // XKILL_IO_LOADER_MD5_WEIGHT_DESC_H