#include <DirectXMath.h>

#include "LoaderFbxAnimationBone.h"

LoaderFbxAnimationBone::LoaderFbxAnimationBone()
{
	keyframes_.resize(NUM_KEYFRAME_TYPES);
}
LoaderFbxAnimationBone::LoaderFbxAnimationBone(const char* name)
{
	keyframes_.resize(NUM_KEYFRAME_TYPES);
	name_ = name;
}
LoaderFbxAnimationBone::~LoaderFbxAnimationBone()
{
}

int LoaderFbxAnimationBone::averageNumKeyframes(FbxKeyframeIndex indices[], int numIndices)
{
	int numKeyframes = 0;
	int numAffectingIndices = 0;

	for(int i=0; i<numIndices; i++)
	{
		if(keyframes_[indices[i]].size()>0)
		{
			numKeyframes += keyframes_[indices[i]].size();
			numAffectingIndices ++;
		}
	}

	if(numAffectingIndices == 0)
		numAffectingIndices = 1;

	return numKeyframes/numAffectingIndices;
}

float LoaderFbxAnimationBone::interpolateValue(FbxKeyframeIndex index, float time)
{
	float value = 0.0f;

	if(keyframes_[index].size() > 0)
	{
		if(time <= keyframes_[index].front().time_)
			value = keyframeFirst(index);
		else if(time >= keyframes_[index].back().time_)
			value = keyframeLast(index);
		else
			value = keyframeInterpolate(index, time);
	}

	return value;
}
float LoaderFbxAnimationBone::keyframeFirst(FbxKeyframeIndex index)
{
	return keyframes_[index].front().value_;
}
float LoaderFbxAnimationBone::keyframeLast(FbxKeyframeIndex index)
{
	return keyframes_[index].back().value_;
}
float LoaderFbxAnimationBone::keyframeInterpolate(FbxKeyframeIndex index, float time)
{
	float interpolatedValue = 0.0f;

	bool done = false;
	unsigned int i=0;
	while(i<keyframes_[index].size()-1 && !done)
	{
		if(time >= keyframes_[index][i].time_ && time <= keyframes_[index][i+1].time_)
		{
			float numerator		= (time - keyframes_[index][i].time_);
			float denumerator	= (keyframes_[index][i+1].time_ - keyframes_[index][i].time_);
			float lerpPercent	= numerator/denumerator;

			float value1 = keyframes_[index][i].value_;
			float value2 = keyframes_[index][i+1].value_;

			interpolatedValue = value1 +(value2-value1)*lerpPercent;
			
			done = true;
		}
	}

	return interpolatedValue;
}


float LoaderFbxAnimationBone::getAnimationStartTime(FbxKeyframeIndex index)
{
	float startTime = 0.0f;
	if(keyframes_[index].size() > 0)
		startTime = keyframes_[index].front().time_;

	return startTime;
}
float LoaderFbxAnimationBone::getAnimationEndTime(FbxKeyframeIndex index)
{
	float endTime = 0.0f;
	if(keyframes_[index].size() > 0)
		endTime = keyframes_[index].back().time_;

	return endTime;
}

const char* LoaderFbxAnimationBone::getName()
{
	return name_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getKeyframes(FbxKeyframeIndex index)
{
	return &(keyframes_[index]);
}


void LoaderFbxAnimationBone::setName(const char* name)
{
	name_ = name;
}
void LoaderFbxAnimationBone::setKeyframes(FbxKeyframeIndex index, std::vector<LoaderFbxAnimationKeyFrame> keyframes)
{
	keyframes_[index] = keyframes;
}