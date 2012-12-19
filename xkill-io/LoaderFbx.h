#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>

#include <fbxsdk.h>
#include <fbxsdk/fbxsdk_compatibility.h>

#define FBXSDK_NEW_API

class LoaderFbx
{
public:
	LoaderFbx();
	~LoaderFbx();

	bool load(std::string filename);

private:
	bool createFbxManager();
	bool createFbxScene();
	bool loadScene(std::string filename);
	
	void parseNode(FbxNode* node);
	void parseMesh(FbxNode* node);
	void parseVertexPositions(FbxMesh* mesh, int polygonVertexCount);
	void parseVertexNormals(FbxMesh* mesh, int polygonCount);

	std::vector<Float3> vertexPositions;
	std::vector<Float3> vertexNormals;

	FbxManager*		fbxManager_;
	FbxScene*		fbxScene_;
};

#endif