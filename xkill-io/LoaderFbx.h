#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>

#include <fbxsdk.h>
#include <fbxsdk/fbxsdk_compatibility.h>


class LoaderFbxMesh;
class LoaderFbxMaterial;
class LoaderFbxMaterialDesc;
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
	
	void parseNode(FbxNode* node);
	void parseMesh(FbxNode* node);
	void parseAnimation(FbxScene* scene);
	

	LoaderFbxMesh* meshLoader_;
	LoaderFbxMaterial* materialLoader_;
	LoaderFbxMaterialDesc* materialDesc_;
	LoaderFbxAnimation* animationLoader_;

	FbxManager*		fbxManager_;
	FbxScene*		fbxScene_;
};

#endif