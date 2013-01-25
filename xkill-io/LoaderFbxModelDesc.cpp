
#include "LoaderFbxModelDesc.h"

LoaderFbxModelDesc::LoaderFbxModelDesc()
{
}
LoaderFbxModelDesc::~LoaderFbxModelDesc()
{
}

LoaderFbxMeshDesc		LoaderFbxModelDesc::getMeshDesc()
{
	return meshDesc_;
}
LoaderFbxMaterialDesc	LoaderFbxModelDesc::getMaterialDesc()
{
	return materialDesc_;
}
LoaderFbxTextureDesc	LoaderFbxModelDesc::getTextureDesc()
{
	return textureDesc_;
}
std::vector<LoaderFbxAnimationDesc>	LoaderFbxModelDesc::getAnimationDescs()
{
	return animationDescs_;
}

void LoaderFbxModelDesc::setMeshDesc(LoaderFbxMeshDesc meshDesc)
{
	meshDesc_ = meshDesc;
}
void LoaderFbxModelDesc::setMaterialDesc(LoaderFbxMaterialDesc materialDesc)
{
	materialDesc_ = materialDesc;
}
void LoaderFbxModelDesc::setTextureDesc(LoaderFbxTextureDesc textureDesc)
{
	textureDesc_ = textureDesc;
}
void LoaderFbxModelDesc::setAnimationDescs(std::vector<LoaderFbxAnimationDesc>	animationDescs)
{
	animationDescs_ = animationDescs;
}