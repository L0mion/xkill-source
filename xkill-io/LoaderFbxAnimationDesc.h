#ifndef XKILL_IO_LOADER_FBX_ANIMATION_DESC_H
#define XKILL_IO_LOADER_FBX_ANIMATION_DESC_H

#include <vector>
#include <map>
#include <string>

#include <xkill-renderer/AnimationClip.h>
#include <xkill-renderer/BoneAnimation.h>
#include <xkill-renderer/Keyframe.h>

#include "LoaderFbxAnimationBone.h"

//! Class for describing a FBX animation.
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxAnimationDesc
{
public:
	//! Initalizes LoaderFbxAnimationDesc to its default state.
	LoaderFbxAnimationDesc();
	//! Initializes LoaderFbxAnimationDesc with specific valies.
	LoaderFbxAnimationDesc(const char* name);
	//! Currently unused.
	~LoaderFbxAnimationDesc();

	const char* getName();
	std::vector<LoaderFbxAnimationBone> getBones();

	void setName(const char* name);
	void setBones(std::vector<LoaderFbxAnimationBone> bones);

	void convertToXKillFormat(std::map<std::string, AnimationClip*>* animations);

private:
	void convertBoneToXKillFormat(LoaderFbxAnimationBone fbxBone, BoneAnimation* bone);
	void createKeyframe(LoaderFbxAnimationBone fbxBone, Keyframe* keyframe, float time);

	unsigned int findMaxNumKeyframes(LoaderFbxAnimationBone bone);
	unsigned int findMinNumKeyframes(LoaderFbxAnimationBone bone);

	const char* name_; //<! Name of the animaiton.

	std::vector<LoaderFbxAnimationBone> bones_; //<! Containing all the bones used in the animation.

};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_DESC_H