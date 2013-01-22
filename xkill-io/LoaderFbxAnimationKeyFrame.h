#ifndef XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H
#define XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H

struct LoaderFbxAnimationKeyFrame
{

	LoaderFbxAnimationKeyFrame();
	LoaderFbxAnimationKeyFrame(float time,
							   float value,
							   int interpolationType,
							   int constantMode,
							   int tangentMode,
							   int weightedMode,
							   int velocityMode);
	
	float time_;
	float value_;

	int interpolationType_;
	int constantMode_;
	int tangentMode_;
	int weightedMode_;
	int velocityMode_;
};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H