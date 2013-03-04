#ifndef XKILL_IO_LOADER_MD5_VERTEX_DESC_H
#define XKILL_IO_LOADER_MD5_VERTEX_DESC_H

#include <DirectXMath.h>

struct LoaderMD5VertexDesc
{
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT3 normal_;
	DirectX::XMFLOAT2 texcoord_;
	int startWeight_;
	int numWeights_;

	LoaderMD5VertexDesc()
	{
		position_	 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		normal_		 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		texcoord_	 = DirectX::XMFLOAT2(0.0f, 0.0f);
		startWeight_ = 0;
		numWeights_  = 0;
	}
	LoaderMD5VertexDesc(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 texcoord, int startWeight, int numWeights)
	{
		position_	 = position;
		normal_		 = normal;
		texcoord_	 = texcoord;
		startWeight_ = startWeight;
		numWeights_	 = numWeights;
	}
};

#endif //XKILL_IO_LOADER_MD5_VERTEX_DESC_H