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
	int numIndices = 9;
	FbxKeyframeIndex keyframeIndices[] = {FBX_KEYFRAME_INDEX_TRANSLATION_X,
										FBX_KEYFRAME_INDEX_TRANSLATION_Y,
										FBX_KEYFRAME_INDEX_TRANSLATION_Z,
										FBX_KEYFRAME_INDEX_ROTATION_X,
										FBX_KEYFRAME_INDEX_ROTATION_Y,
										FBX_KEYFRAME_INDEX_ROTATION_Z,
										FBX_KEYFRAME_INDEX_SCALING_X,
										FBX_KEYFRAME_INDEX_SCALING_Y,
										FBX_KEYFRAME_INDEX_SCALING_Z};

	for(unsigned int boneIndex=0; boneIndex<bones_.size(); boneIndex++)
	{
		int numKeyframes = bones_[boneIndex].averageNumKeyframes(keyframeIndices, numIndices);
		int debug = 1;
	}
}
unsigned int LoaderFbxAnimationDesc::findMaxNumKeyframes(LoaderFbxAnimationBone bone)
{
	unsigned int numKeyframes = 0;
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_X)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Y)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Z)->size());
	
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_X)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Y)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Z)->size());

	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_X)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Y)->size());
	numKeyframes = DirectX::XMMax(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Z)->size());

	return numKeyframes;
}
unsigned int LoaderFbxAnimationDesc::findMinNumKeyframes(LoaderFbxAnimationBone bone)
{
	unsigned int numKeyframes = UINT_MAX;
	bool nonZero = false;
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_X)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_X)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Y)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Y)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Z)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Z)->size());
	}
						
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_X)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_X)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Y)->size() > 0)	
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Y)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Z)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Z)->size());
	}
		
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_X)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_X)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Y)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Y)->size());
	}
	if(bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Z)->size() > 0)
	{
		nonZero = true;
		numKeyframes = DirectX::XMMin(numKeyframes, bone.getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Z)->size());
	}

	if(!nonZero)
		numKeyframes = 0;

	return numKeyframes;
}