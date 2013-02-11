#ifndef XKILL_IO_LOADERFBXTEXTURE_H
#define XKILL_IO_LOADERFBXTEXTURE_H

#include <vector>

namespace fbxsdk_2014_0_beta2
{
	class FbxGeometry;
	class FbxTexture;
	class FbxProperty;
}

class LoaderFbxTextureDesc;

//! Class for loading a FBX texture
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxTexture
{
public:
	//! Initializes LoaderFbxTexture to its default state.
	LoaderFbxTexture();
	//! Is unused.
	~LoaderFbxTexture();
	//! Is unused.
	void reset();

	//! Retieves texture data from a FbxGeometry object.
	/*!
	\param geometry A FbxGeometry object from where the texture data will be retrieved.
	\param textureDesc Texture description where the results will be stored.
	*/
	void parseTexture(FbxGeometry* geometry, std::vector<LoaderFbxTextureDesc>* textureDesc);
private:
	//! Retrieve texture data from a FbxProperty.
	/*!
	\param fbxProperty The proprty containing the texture data.
	\param parseHeader Currently unused.
	\param materialIndex Currently unused.
	\param textureDesc Texture description where the results will be stored.
	*/
	void parseTextureByProperty(FbxProperty* fbxProperty, bool& parseHeader, int materialIndex, std::vector<LoaderFbxTextureDesc>* textureDesc);
	//! Retrieve a layered texture from the property.
	/*!
	\param fbxProperty The proprty containing the texture data.
	\param textureIndex Index to the current texture.
	\param textureDesc Texture description where the results will be stored.
	*/
	void parseTextureByPropertyLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc);
	//! Retrieve a texture that is not layered from the property.
	/*!
	\param fbxProperty The proprty containing the texture data.
	\param textureIndex Index to the current texture.
	\param textureDesc Texture description where the results will be stored.
	*/
	void parseTextureByPropertyNotLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc);
	//! Parse data from a FbxTexture object
	/*!
	\param texture Texture object to be parsed.
	\param blendMode Determines how layered textures are blended. 
	\param textureDesc Texture description where the results will be stored.
	*/
	void parseTextureInfo(FbxTexture* texture, int blendMode, LoaderFbxTextureDesc* textureDesc);
};

#endif //XKILL_IO_LOADERFBXTEXTURE_H