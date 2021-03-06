
#include <DirectXMath.h>

#include "AnimationClip.h"
#include "BoneAnimation.h"

AnimationClip::AnimationClip()
{
	boneAnimations_ = new std::vector<BoneAnimation*>();
}

AnimationClip::~AnimationClip()
{
	for(unsigned int i=0; i<boneAnimations_->size(); i++)
	{
		if(boneAnimations_->at(i))
			delete boneAnimations_->at(i);
	}
	delete boneAnimations_;
}

void AnimationClip::interpolate(float time, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const
{
	for(unsigned int i=0; i<boneAnimations_->size(); i++)
		boneAnimations_->at(i)->interpolate(time, boneTransforms[i]);
}

float AnimationClip::getClipStartTime() const
{
	float startTime = FLT_MAX;
	for(unsigned int i=0; i<boneAnimations_->size(); i++)
		startTime = DirectX::XMMin<float>(startTime, boneAnimations_->at(i)->getStartTime());

	return startTime;
}

float AnimationClip::getClipEndTime() const
{
	float endTime = 0.0f;
	for(unsigned int i=0; i<boneAnimations_->size(); i++)
		endTime = DirectX::XMMax<float>(endTime, boneAnimations_->at(i)->getEndTime());

	return endTime;
}

std::vector<BoneAnimation*>* AnimationClip::getBoneAnimations() const
{
	return boneAnimations_;
}

void AnimationClip::setBoneAnimations(std::vector<BoneAnimation*>* boneAnimations)
{
	if(boneAnimations_)
		delete boneAnimations_;

	boneAnimations_ = boneAnimations;
}