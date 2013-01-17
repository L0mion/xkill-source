#include "LoaderFbxMeshDesc.h"

LoaderFbxMeshDesc::LoaderFbxMeshDesc()
{
}

LoaderFbxMeshDesc::LoaderFbxMeshDesc(std::vector<int>	polygonGroupIds,
	std::vector<Float3> vertexPositions,
	std::vector<Float4> vertexColors,
	std::vector<Float3> vertexNormals,
	std::vector<Float2> vertexUVs,
	std::vector<Float4> vertexTangents,
	std::vector<Float4> vertexBinormals)
{
	polygonGroupIds_	= polygonGroupIds;
	vertexColors_		= vertexColors;
	vertexNormals_		= vertexNormals;
	vertexUVs_			= vertexUVs;
	vertexTangents_		= vertexTangents;
	vertexBinormals_	= vertexBinormals;
}
LoaderFbxMeshDesc::~LoaderFbxMeshDesc()
{
}


void LoaderFbxMeshDesc::setPolygonGroupIds(std::vector<int> polygonGroupIds)
{
	polygonGroupIds_ = polygonGroupIds;
}
void LoaderFbxMeshDesc::setVertexPositions(std::vector<Float3> vertexPositions)
{
	vertexPositions_ = vertexPositions;
}
void LoaderFbxMeshDesc::setVertexColors(std::vector<Float4> vertexColors)
{
	vertexColors_ = vertexColors;
}
void LoaderFbxMeshDesc::setVertexNormals(std::vector<Float3> vertexNormals)
{
	vertexNormals_ = vertexNormals;
}
void LoaderFbxMeshDesc::setVertexUVs(std::vector<Float2> vertexUVs)
{
	vertexUVs_ = vertexUVs;
}
void LoaderFbxMeshDesc::setVertexTangents(std::vector<Float4> vertexTangents)
{
	vertexTangents_ = vertexTangents;
}
void LoaderFbxMeshDesc::setVertexBinormals(std::vector<Float4> vertexBinormals)
{
	vertexBinormals_ = vertexBinormals;
}

std::vector<int>	LoaderFbxMeshDesc::getPolygonGroupIds()
{
	return polygonGroupIds_;
}
std::vector<Float3> LoaderFbxMeshDesc::getVertexPositions()
{
	return vertexPositions_;
}
std::vector<Float4> LoaderFbxMeshDesc::getVertexColors()
{
	return vertexColors_;
}
std::vector<Float3>	LoaderFbxMeshDesc::getVertexNormals()
{
	return vertexNormals_;
}
std::vector<Float2>	LoaderFbxMeshDesc::getVertexUVs()
{
	return vertexUVs_;
}
std::vector<Float4>	LoaderFbxMeshDesc::getVertexTangents()
{
	return vertexTangents_;
}
std::vector<Float4>	LoaderFbxMeshDesc::getVertexBinormals()
{
	return vertexBinormals_;
}