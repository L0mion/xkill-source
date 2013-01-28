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
void  LoaderFbxAnimationDesc::setBones(std::vector<LoaderFbxAnimationBone> bones)
{
	bones_ = bones;
}