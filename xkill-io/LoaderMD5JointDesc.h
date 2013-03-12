#ifndef XKILL_IO_LOADER_MD5_JOINT_DESC_H
#define XKILL_IO_LOADER_MD5_JOINT_DESC_H

#include <string>
#include <DirectXMath.h>

struct LoaderMD5JointDesc
{
	std::string			name_;
	int					parentID_;
	DirectX::XMFLOAT3	position_;
	DirectX::XMFLOAT4	orientationQuaternion_;

	LoaderMD5JointDesc()
	{
		name_					= "Unknown";
		parentID_				= -1;
		position_				= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		orientationQuaternion_	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	LoaderMD5JointDesc(std::string name, int parentID, DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 orientationQuaternion)
	{
		name_					= name;
		parentID_				= parentID;
		position_				= position;
		orientationQuaternion_	= orientationQuaternion;
	}
};

#endif //XKILL_IO_LOADER_MD5_JOINT_DESC_H