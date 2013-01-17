#ifndef XKILL_IO_LOADERFBXTEXTURE_H
#define XKILL_IO_LOADERFBXTEXTURE_H

namespace fbxsdk_2014_0_beta2
{
	class FbxGeometry;
	class FbxTexture;
	class FbxProperty;
}


class LoaderFbxTexture
{
public:
	LoaderFbxTexture();
	~LoaderFbxTexture();
	void reset();

	void parseTexture(FbxGeometry* geometry);
private:
	void parseTextureByProperty(FbxProperty* fbxProperty, bool& parseHeader, int materialIndex);
	void parseTextureByPropertyLayered(FbxProperty* fbxProperty, int textureIndex);
	void parseTextureByPropertyNotLayered(FbxProperty* fbxProperty, int textureIndex);
	void parseTextureInfo(FbxTexture* texture, int blendMode);
};

#endif //XKILL_IO_LOADERFBXTEXTURE_H