#ifndef XKILL_IO_LOADER_MD5_ANIMATION_H
#define XKILL_IO_LOADER_MD5_ANIMATION_H

#include <vector>
#include <fstream>

#include "LoaderMD5AnimationDesc.h"
#include "LoaderMD5JointInfo.h"
#include "LoaderMD5BoundsDesc.h"
#include "LoaderMD5BaseFrameDesc.h"
#include "LoaderMD5FrameDataDesc.h"
#include "LoaderMD5FrameSkeletonDesc.h"

class LoaderMD5Animation
{
public:
	LoaderMD5Animation();
	~LoaderMD5Animation();

	bool loadAnimation(const std::string& filename, LoaderMD5AnimationDesc* animationDesc);

private:
	
	void parseParamMD5Version();
	void parseParamCommandline();
	void parseParamNumFrames();
	void parseParamNumJoints();
	void parseParamFrameRate();
	void parseParamNumAnimatedComponents();
	void parseParamHierarchy();
	void parseParamBounds();
	void parseParamBaseFrame();
	void parseParamFrame();

	void buildFrameSkeleton(std::vector<LoaderMD5FrameSkeleton>& skeletons, const std::vector<LoaderMD5JointInfo>& jointInfos,
							const std::vector<LoaderMD5BaseFrameDesc>& baseFrames, const LoaderMD5FrameDataDesc& frameData);

	void reset();

	std::ifstream infile_;
	int fileLength_;

	std::vector<LoaderMD5JointInfo>		jointInfos_;
	std::vector<LoaderMD5BoundsDesc>	bounds_;
	std::vector<LoaderMD5BaseFrameDesc> baseFrames_;
	std::vector<LoaderMD5FrameDataDesc> frames_;
	std::vector<LoaderMD5FrameSkeleton> skeletons_;
	
	LoaderMD5FrameSkeleton animatedSkeleton_;

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