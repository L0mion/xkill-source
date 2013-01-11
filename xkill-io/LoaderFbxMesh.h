#ifndef XKILL_IO_LOADERFBXMESH_H
#define XKILL_IO_LOADERFBXMESH_H

#include <vector>
#include <fbxsdk.h>
#include <xkill-utilities/Math.h>

static const unsigned int POLYGON_SIZE = 3;

class LoaderFbxMesh
{
public:
	LoaderFbxMesh();
	~LoaderFbxMesh();
	void reset();

	void parseMesh(FbxMesh* mesh);
	void createVerticesPosNormTex();
	std::vector<unsigned int> getIndices();
	std::vector<VertexPosNormTex> getVerticesPosNormTex();

private:
	void parseVertexPositions(FbxMesh* mesh, int polygonVertexCount);
	void parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId);
	void parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex);
	void parseVertexTangents(FbxMesh* mesh, int vertexId);
	void parseVertexBinormals(FbxMesh* mesh, int vertexId);

	bool float2Equal(Float2 f1, Float2 f2);
	bool float3Equal(Float3 f1, Float3 f2);
	bool float4Equal(Float4 f1, Float4 f2);

	std::vector<VertexPosNormTex> assemblePosNormTex();
	void indexPosNormTex(std::vector<VertexPosNormTex> posNormTex);
	
	std::vector<unsigned int>	indices_;
	std::vector<VertexPosNormTex> verticesPosNormTex_;

	std::vector<Float3> vertexPositions_;
	std::vector<Float3> vertexNormals_;
	std::vector<Float2> vertexUVs_;
	std::vector<Float4> vertexTangents_;
	std::vector<Float4> vertexBinormals_;
};

#endif