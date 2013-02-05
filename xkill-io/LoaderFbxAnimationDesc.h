#ifndef XKILL_IO_LOADER_FBX_ANIMATION_DESC_H
#define XKILL_IO_LOADER_FBX_ANIMATION_DESC_H

#include <vector>

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

	void convertToXKillFormat();

private:
	unsigned int findMaxNumKeyframes(LoaderFbxAnimationBone bone);
	unsigned int findMinNumKeyframes(LoaderFbxAnimationBone bone);

	const char* name_; //<! Name of the animaiton.

	std::vector<LoaderFbxAnimationBone> bones_; //<! Containing all the bones used in the animation.

};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_DESC_H