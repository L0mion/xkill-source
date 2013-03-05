#ifndef XKILL_IO_LOADER_MD5_ANIMATION_H
#define XKILL_IO_LOADER_MD5_ANIMATION_H

#include <vector>

#include "LoaderMD5JointInfo.h"
#include "LoaderMD5BoundsDesc.h"
#include "LoaderMD5BaseFrameDesc.h"
#include "LoaderMD5FrameDataDesc.h"
#include "LoaderMD5FrameSkeletonDesc.h"

class LoaderMD5Animation
{
public:
private:
	

	int md5Version_;
	int numFrames_;
	int numJoints_;
	int frameRate_;
	int numAnimatedComponents_;

	float animationDuration_;
	float frameDuration_;
	float animationTime_;
};

#endif //XKILL_IO_LOADER_MD5_ANIMATION_H