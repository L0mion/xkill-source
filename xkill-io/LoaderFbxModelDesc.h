#ifndef XKILL_IO_LOADER_FBX_MODEL_DESC_H
#define XKILL_IO_LOADER_FBX_MODEL_DESC_H

#include <vector>

#include "LoaderFbxMeshDesc.h"
#include "LoaderFbxMaterialDesc.h"
#include "LoaderFbxTextureDesc.h"
#include "LoaderFbxAnimationDesc.h"

//! Class for storing texture data.
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxModelDesc
{
public:
	//! Initializes LoaderFbxModelDesc to its default state.
	LoaderFbxModelDesc();
	//! Currently unused.
	~LoaderFbxModelDesc();

	LoaderFbxMeshDesc					getMeshDesc();
	LoaderFbxMaterialDesc				getMaterialDesc();
	LoaderFbxTextureDesc				getTextureDesc();
	std::vector<LoaderFbxAnimationDesc>	getAnimationDescs();

	void setMeshDesc(LoaderFbxMeshDesc meshDesc);
	void setMaterialDesc(LoaderFbxMaterialDesc materialDesc);
	void setTextureDesc(LoaderFbxTextureDesc textureDesc);
	void setAnimationDescs(std::vector<LoaderFbxAnimationDesc> animationDescs);
private:
	LoaderFbxMeshDesc					meshDesc_;		 //<! Describing mesh data.
	LoaderFbxMaterialDesc				materialDesc_;	 //<! Describing material data.
	LoaderFbxTextureDesc				textureDesc_;	 //<! Describing texture data.
	std::vector<LoaderFbxAnimationDesc>	animationDescs_; //<! Describing animation data.
};

#endif // XKILL_IO_LOADER_FBX_MODEL_DESC_H