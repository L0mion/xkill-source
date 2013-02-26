#include <map>
#include <string>

#include <DirectXMath.h>

#include <xkill-utilities/AnimationClip.h>
#include <xkill-utilities/BoneAnimation.h>
#include <xkill-utilities/Keyframe.h>

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

void LoaderFbxAnimationDesc::convertToXKillFormat(std::map<std::string, AnimationClip*>* animations)
{
	AnimationClip* animationClip = new AnimationClip();
	animationClip->getBoneAnimations()->resize(bones_.size());
	
	for(unsigned int boneIndex=0; boneIndex<bones_.size(); boneIndex++)
	{
		animationClip->getBoneAnimations()->at(boneIndex) = new BoneAnimation();

		convertBoneToXKillFormat(bones_[boneIndex], animationClip->getBoneAnimations()->at(boneIndex));
	}

	std::pair<std::string, AnimationClip*> animation(name_, animationClip);
	animations->insert(animation);
}
void LoaderFbxAnimationDesc::convertBoneToXKillFormat(LoaderFbxAnimationBone fbxBone, BoneAnimation* bone)
{
	//int numIndices = 9;
	FbxKeyframeIndex keyframeIndices[] = {FBX_KEYFRAME_INDEX_TRANSLATION_X,
										  FBX_KEYFRAME_INDEX_TRANSLATION_Y,
										  FBX_KEYFRAME_INDEX_TRANSLATION_Z,
										  FBX_KEYFRAME_INDEX_ROTATION_X,
										  FBX_KEYFRAME_INDEX_ROTATION_Y,
										  FBX_KEYFRAME_INDEX_ROTATION_Z,
										  FBX_KEYFRAME_INDEX_SCALING_X,
										  FBX_KEYFRAME_INDEX_SCALING_Y,
										  FBX_KEYFRAME_INDEX_SCALING_Z};

	int numKeyframes = findMaxNumKeyframes(fbxBone);//fbxBone.averageNumKeyframes(keyframeIndices, numIndices);
	bone->getKeyframes()->resize(numKeyframes);

	if(numKeyframes > 0)
	{
		float startTime = fbxBone.getAnimationStartTime(FBX_KEYFRAME_INDEX_TRANSLATION_X);
		float endTime	= fbxBone.getAnimationEndTime(FBX_KEYFRAME_INDEX_TRANSLATION_X);
		float timeInterval = (endTime-startTime)/numKeyframes;
		float time = startTime;

		for(int i=0; i<numKeyframes; i++)
		{
			bone->getKeyframes()->at(i) = new Keyframe();
			createKeyframe(fbxBone, bone->getKeyframes()->at(i), time);
			time += timeInterval;
		}
	}
	else
	{
		Keyframe* keyframe				= new Keyframe();
		keyframe->translation			= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		keyframe->scale					= DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		keyframe->rotationQuaternion	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		keyframe->timePosition			= 0.0f;
		bone->getKeyframes()->push_back(keyframe);
	}
}
void LoaderFbxAnimationDesc::createKeyframe(LoaderFbxAnimationBone fbxBone, Keyframe* keyframe, float time)
{
	float translationX = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_TRANSLATION_X, time);
	float translationY = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_TRANSLATION_Y, time);
	float translationZ = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_TRANSLATION_Z, time);
	
	float rotationX = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_ROTATION_X, time);
	float rotationY = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_ROTATION_Y, time);
	float rotationZ = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_ROTATION_Z, time);
	
	float scalingX = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_SCALING_X, time);
	float scalingY = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_SCALING_Y, time);
	float scalingZ = fbxBone.interpolateValue(FBX_KEYFRAME_INDEX_SCALING_Z, time);
	if(scalingX == 0 && scalingY == 0 && scalingZ == 0)
	{
		scalingX = 1.0f;
		scalingY = 1.0f;
		scalingZ = 1.0f;
	}
	
	DirectX::XMVECTOR xmTemp = DirectX::XMQuaternionRotationRollPitchYaw(rotationZ * (DirectX::XM_PI / 180.0f), rotationY * (DirectX::XM_PI / 180.0f), rotationX * (DirectX::XM_PI / 180.0f));
	DirectX::XMFLOAT4 temp;
	DirectX::XMStoreFloat4(&temp, xmTemp);

	DirectX::XMMATRIX xmRotationX = DirectX::XMMatrixRotationX(-rotationX * (DirectX::XM_PI / 180.0f));
	DirectX::XMMATRIX xmRotationY = DirectX::XMMatrixRotationY(rotationY * (DirectX::XM_PI / 180.0f));
	DirectX::XMMATRIX xmRotationZ = DirectX::XMMatrixRotationZ(rotationZ * (DirectX::XM_PI / 180.0f));
	
	DirectX::XMMATRIX xmRotation = xmRotationZ * xmRotationY * xmRotationX;
	DirectX::XMVECTOR xmQuaternion = DirectX::XMQuaternionRotationMatrix(xmRotation);
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMQuaternionNormalize(xmQuaternion);
	DirectX::XMStoreFloat4(&quaternion, xmQuaternion);

	keyframe->translation			= DirectX::XMFLOAT3(translationX, translationY, translationZ);
	keyframe->scale					= DirectX::XMFLOAT3(scalingX, scalingY, scalingZ);
	keyframe->rotationQuaternion	= quaternion;
	keyframe->timePosition			= time;
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