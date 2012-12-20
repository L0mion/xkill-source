#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>

#include <fbxsdk.h>
#include <fbxsdk/fbxsdk_compatibility.h>

#define FBXSDK_NEW_API

static const unsigned int POLYGON_SIZE = 3;

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
	void parseIndices(FbxMesh* mesh, int polygonVertexCount);
	void parseVertexPositions(FbxMesh* mesh, int polygonVertexCount);
	
	void parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId);

	std::vector<int>	indices_;
	std::vector<Float3> vertexPositions_;
	std::vector<Float3> vertexNormals_;

	FbxManager*		fbxManager_;
	FbxScene*		fbxScene_;
};

#endif