
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
std::vector<LoaderFbxTextureDesc>	LoaderFbxModelDesc::getTextureDescs()
{
	return textureDescs_;
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
void LoaderFbxModelDesc::setTextureDescs(std::vector<LoaderFbxTextureDesc> textureDescs)
{
	textureDescs_ = textureDescs;
}
void LoaderFbxModelDesc::setAnimationDescs(std::vector<LoaderFbxAnimationDesc>	animationDescs)
{
	animationDescs_ = animationDescs;
}