#include <DirectXMath.h>

#include "SkinnedData.h"
#include "AnimationClip.h"


SkinnedData::SkinnedData()
{
	boneHierarchy_	= new std::vector<int>();
	boneOffsets_	= new std::vector<DirectX::XMFLOAT4X4>();
	animations_		= new std::map<std::string, AnimationClip*>();
}

SkinnedData::~SkinnedData()
{
	if(boneHierarchy_)
		delete boneHierarchy_;
	if(boneOffsets_)
		delete boneOffsets_;
	
	if(animations_)
	{
		std::map<std::string, AnimationClip*>::iterator index;
		for(index = animations_->begin(); index != animations_->end(); index++)
		{
			if(index->second)
				delete index->second;
		}
		if(animations_)
			delete animations_;
	}
}

void SkinnedData::set(std::vector<int>*						 boneHierarchy,
					   std::vector<DirectX::XMFLOAT4X4>*	 boneOffsets,
					   std::map<std::string, AnimationClip*>* animations)
{
	if(boneHierarchy_)
		delete boneHierarchy_;
	if(boneOffsets_)
		delete boneOffsets_;
	
	if(animations_)
	{
		std::map<std::string, AnimationClip*>::iterator index;
		for(index = animations_->begin(); index != animations_->end(); index++)
		{
			if(index->second)
				delete index->second;
		}
		if(animations_)
			delete animations_;
	}

	boneHierarchy_	= boneHierarchy;
	boneOffsets_	= boneOffsets;
	animations_		= animations;
}

void SkinnedData::getFinalTransforms(const std::string&					clipName,
									 float								timePosition,
									 std::vector<DirectX::XMFLOAT4X4>*	finalTransforms) const
{
	unsigned int numBones = boneOffsets_->size();

	finalTransforms->resize(numBones);

	std::vector<DirectX::XMFLOAT4X4> toParentTransforms(numBones);

	auto clip = animations_->find(clipName);
	clip->second->interpolate(timePosition, toParentTransforms);

	std::vector<DirectX::XMFLOAT4X4> toRootTransforms(numBones);

	toRootTransforms[0] = toParentTransforms[0];

	for(unsigned int i=1; i<numBones; i++)
	{
		DirectX::XMMATRIX toParent = DirectX::XMLoadFloat4x4(&toParentTransforms[i]);
		int parentIndex = boneHierarchy_->at(i);
		DirectX::XMMATRIX parentToRoot = DirectX::XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);

		DirectX::XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	for(unsigned int i=0; i<numBones; i++)
	{
		DirectX::XMMATRIX offset = DirectX::XMLoadFloat4x4(&boneOffsets_->at(i));
		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&toRootTransforms[i]);
		DirectX::XMStoreFloat4x4(&finalTransforms->at(i), DirectX::XMMatrixMultiply(offset, toRoot));
	}
}

unsigned int SkinnedData::getBoneCount() const
{
	return boneHierarchy_->size();
}

float SkinnedData::getClipStartTime(const std::string& clipName) const
{
	auto clip = animations_->find(clipName);
	return clip->second->getClipStartTime();
}
float SkinnedData::getClipEndTime(const std::string& clipName) const
{
	auto clip = animations_->find(clipName);
	return clip->second->getClipEndTime();
}

std::map<std::string, AnimationClip*>* SkinnedData::getAnimations() const
{
	return animations_;
}