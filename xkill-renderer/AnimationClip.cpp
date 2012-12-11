
#include <DirectXMath.h>

#include "AnimationClip.h"

AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::interpolate(float time, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const
{
	for(unsigned int i=0; i<boneAnimations_.size(); i++)
		boneAnimations_[i].interpolate(time, boneTransforms[i]);
}

float AnimationClip::getClipStartTime() const
{
	float startTime = FLT_MAX;
	for(unsigned int i=0; i<boneAnimations_.size(); i++)
		startTime = DirectX::XMMin<float>(startTime, boneAnimations_[i].getStartTime());

	return startTime;
}

float AnimationClip::getClipEndTime() const
{
	float endTime = FLT_MIN;
	for(unsigned int i=0; i<boneAnimations_.size(); i++)
		endTime = DirectX::XMMax<float>(endTime, boneAnimations_[i].getEndTime());

	return endTime;
}