#ifndef XKILL_IO_LOADER_MD5_SKELETON_JOINT_DESC_H
#define XKILL_IO_LOADER_MD5_SKELETON_JOINT_DESC_H

#include <DirectXMath.h>

struct LoaderMD5SkeletonJointDesc
{
	int parent_;
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT4 orientationQuaternion_;

	LoaderMD5SkeletonJointDesc()
	{
		parent_ = -1;
		position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		orientationQuaternion_ = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	LoaderMD5SkeletonJointDesc(int parent, DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 orientationQuaternion)
	{
		parent_ = parent;
		position_ = position;
		orientationQuaternion_ = orientationQuaternion;
	}
	LoaderMD5SkeletonJointDesc(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 orientationQuaternion)
	{
		position_ = position;
		orientationQuaternion_ = orientationQuaternion;
	}
};

#endif //XKILL_IO_LOADER_MD5_SKELETON_JOINT_DESC_H