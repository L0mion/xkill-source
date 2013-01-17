#ifndef XKILL_IO_LOADERFBXTEXTURE_H
#define XKILL_IO_LOADERFBXTEXTURE_H

namespace fbxsdk_2014_0_beta2
{
	class FbxGeometry;
	class FbxTexture;
	class FbxProperty;
}

class LoaderFbxTextureDesc;

class LoaderFbxTexture
{
public:
	LoaderFbxTexture();
	~LoaderFbxTexture();
	void reset();

	void parseTexture(FbxGeometry* geometry, LoaderFbxTextureDesc* textureDesc);
private:
	void parseTextureByProperty(FbxProperty* fbxProperty, bool& parseHeader, int materialIndex, LoaderFbxTextureDesc* textureDesc);
	void parseTextureByPropertyLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc);
	void parseTextureByPropertyNotLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc);
	void parseTextureInfo(FbxTexture* texture, int blendMode, LoaderFbxTextureDesc* textureDesc);
};

#endif //XKILL_IO_LOADERFBXTEXTURE_H