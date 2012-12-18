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
	void initFbxManager();
	void initFbxIOSettings();
	bool initFbxImporter(std::string filename);
	void initFbxScene();
	
	void	parseNode(FbxNode* node);
	void	parseMesh(FbxMesh* mesh);
	void	parsePolyGroups(FbxMesh* mesh, int polygonIndex);
	int		parseIndex(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex);
	void	parseVertexColor(FbxMesh* mesh, int controlPointIndex, int vertexId);
	void	parseTexcoords(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex);
	void	parseNormals(FbxMesh* mesh, int vertexId);


	FbxString getAttributeTypeName(FbxNodeAttribute::EType type);
	void printTabs();
	void printAttribute(FbxNodeAttribute* attribute);
	void printNode(FbxNode* node);

	int numTabs_;
	std::vector<unsigned int>	indices_;
	std::vector<unsigned int>	polygonGroups_;
	std::vector<FbxColor>		vertexColors_;
	std::vector<FbxVector2>		texcoords_;


	FbxManager*		fbxManager_;
	FbxIOSettings*	fbxIOSettings_;
	FbxImporter*	fbxImporter_;
	FbxScene*		fbxScene_;
};

#endif