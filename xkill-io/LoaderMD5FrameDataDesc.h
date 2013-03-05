#ifndef XKILL_IO_LOADER_MD5_FRAME_DATA_DESC_H
#define XKILL_IO_LOADER_MD5_FRAME_DATA_DESC_H

#include <vector>

struct LoaderMD5FrameDataDesc
{
	int frameID_;
	std::vector<float> frameData_;

	LoaderMD5FrameDataDesc()
	{
		frameID_ = 0;
		frameData_.resize(0);
	}
	LoaderMD5FrameDataDesc(int frameID, std::vector<float> frameData)
	{
		frameID_ = frameID;
		frameData_ = frameData;
	}
};

#endif //XKILL_IO_LOADER_MD5_FRAME_DATA_DESC_H