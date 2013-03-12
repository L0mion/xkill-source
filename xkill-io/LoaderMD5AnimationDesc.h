#ifndef XKILL_IO_LOADER_MD5_ANIMATION_DESC_H
#define XKILL_IO_LOADER_MD5_ANIMATION_DESC_H

#include <vector>

#include "LoaderMD5FrameSkeletonDesc.h"
#include "LoaderMD5BaseFrameDesc.h"
#include "LoaderMD5JointInfo.h"

struct LoaderMD5AnimationDesc
{
	float frameDuration_;

	std::vector<LoaderMD5FrameSkeleton> skeletons_;
	std::vector<LoaderMD5BaseFrameDesc> baseFrames_;
	std::vector<LoaderMD5JointInfo>		jointInfos_;
};

#endif //XKILL_IO_LOADER_MD5_ANIMATION_DESC_H