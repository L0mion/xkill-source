#ifndef XKILL_IO_LOADER_FBX_ANIMATION_DESC_H
#define XKILL_IO_LOADER_FBX_ANIMATION_DESC_H

#include <vector>

#include "LoaderFbxAnimationBone.h"


class LoaderFbxAnimationDesc
{
public:
	LoaderFbxAnimationDesc();
	LoaderFbxAnimationDesc(const char* name);
	~LoaderFbxAnimationDesc();

	const char* getName();
	std::vector<LoaderFbxAnimationBone> getBones();

	void setName(const char* name);
	void setBones(std::vector<LoaderFbxAnimationBone> bones);

private:
	const char* name_;

	std::vector<LoaderFbxAnimationBone> bones_;

};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_DESC_H