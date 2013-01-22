#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>



namespace fbxsdk_2014_0_beta2
{
	class FbxManager;
	class FbxScene;
	class FbxNode;
}

class LoaderFbxMesh;
class LoaderFbxMaterial;
class LoaderFbxMaterialDesc;
class LoaderFbxTexture;
class LoaderFbxAnimation;
struct VertexPosNormTex;

class LoaderFbx
{
public:
	LoaderFbx();
	~LoaderFbx();

	bool init();
	bool load(std::string filename);
	
private:
	bool createFbxManager();
	bool createFbxScene();
	bool loadScene(std::string filename);
	
	void parseNode(fbxsdk_2014_0_beta2::FbxNode* node);
	void parseMesh(fbxsdk_2014_0_beta2::FbxNode* node);
	
	LoaderFbxMesh*			meshLoader_;
	LoaderFbxMaterial*		materialLoader_;
	LoaderFbxTexture*		textureLoader_;
	LoaderFbxAnimation*		animationLoader_;

	fbxsdk_2014_0_beta2::FbxManager*		fbxManager_;
	fbxsdk_2014_0_beta2::FbxScene*		fbxScene_;
};

#endif