#include <xkill-renderer/BoneAnimation.h>
#include <xkill-renderer/Keyframe.h>

#include "LoaderFbxAnimationDesc.h"
#include "LoaderFbxAnimationBone.h"


LoaderFbxAnimationDesc::LoaderFbxAnimationDesc()
{

}
LoaderFbxAnimationDesc::LoaderFbxAnimationDesc(const char* name)
{
	name_ = name;

}
LoaderFbxAnimationDesc::~LoaderFbxAnimationDesc()
{
}

const char* LoaderFbxAnimationDesc::getName()
{
	return name_;
}
std::vector<LoaderFbxAnimationBone> LoaderFbxAnimationDesc::getBones()
{
	return bones_;
}

void LoaderFbxAnimationDesc::setName(const char* name)
{
	name_ = name;
}
void LoaderFbxAnimationDesc::setBones(std::vector<LoaderFbxAnimationBone> bones)
{
	bones_ = bones;
}

void LoaderFbxAnimationDesc::convertToXKillFormat()
{
	for(unsigned int boneIndex=0; boneIndex<bones_.size(); boneIndex++)
	{
		
	}
}
unsigned int LoaderFbxAnimationDesc::findMaxNumKeyframes(LoaderFbxAnimationBone bone)
{
	unsigned int numKeyframes = 0;
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getTranslationX()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getTranslationY()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getTranslationZ()->size());
	
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getScalingX()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getScalingY()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getScalingZ()->size());

	numKeyframes = DirectX::XMMax(numKeyframes, bone.getRotationX()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getRotationY()->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getRotationZ()->size());

	return numKeyframes;
}
unsigned int LoaderFbxAnimationDesc::findMinNumKeyframes(LoaderFbxAnimationBone bone)
{
	unsigned int numKeyframes = UINT_MAX;
	bool nonZero = false;
	if(bone.getTranslationX()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getTranslationX()->size());
	}
	if(bone.getTranslationY()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getTranslationY()->size());
	}
	if(bone.getTranslationZ()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getTranslationZ()->size());
	}
						
	if(bone.getScalingX()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getScalingX()->size());
	}
	if(bone.getScalingY()->size() > 0)	
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getScalingY()->size());
	}
	if(bone.getScalingZ()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getScalingZ()->size());
	}
		
	if(bone.getRotationX()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getRotationX()->size());
	}
	if(bone.getRotationY()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getRotationY()->size());
	}
	if(bone.getRotationZ()->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getRotationZ()->size());
	}

	if(!nonZero)
		numKeyframes = 0;

	return numKeyframes;
}