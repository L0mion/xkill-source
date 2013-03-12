#ifndef XKILL_IO_LOADER_MD5_FRAME_SKELETON_DESC_H
#define XKILL_IO_LOADER_MD5_FRAME_SKELETON_DESC_H

#include <vector>

#include "LoaderMD5SkeletonJointDesc.h"

struct LoaderMD5FrameSkeleton
{
	std::vector<LoaderMD5SkeletonJointDesc> joints_;

	LoaderMD5FrameSkeleton()
	{
		joints_.resize(0);
	}
	LoaderMD5FrameSkeleton(std::vector<LoaderMD5SkeletonJointDesc> joints)
	{
		joints_ = joints;
	}
};

#endif //XKILL_IO_LOADER_MD5_FRAME_SKELETON_DESC_H