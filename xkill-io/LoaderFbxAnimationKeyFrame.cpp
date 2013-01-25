#include "LoaderFbxAnimationKeyFrame.h"

LoaderFbxAnimationKeyFrame::LoaderFbxAnimationKeyFrame()
{
	time_				= 0;
	value_				= 0;
	interpolationType_  = 0;
	constantMode_		= 0;
	weightedMode_		= 0;
	velocityMode_		= 0;
}
LoaderFbxAnimationKeyFrame::LoaderFbxAnimationKeyFrame(float time,
													   float value,
													   int interpolationType,
													   int constantMode,
													   int tangentMode,
													   int weightedMode,
													   int velocityMode)
{
	time_				= time;
	value_				= value;
	interpolationType_	= interpolationType;
	constantMode_		= constantMode;
	tangentMode_		= tangentMode;
	weightedMode_		= weightedMode;
	velocityMode_		= velocityMode;
}