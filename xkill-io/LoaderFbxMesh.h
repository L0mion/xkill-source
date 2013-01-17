#ifndef XKILL_IO_LOADERFBXMESH_H
#define XKILL_IO_LOADERFBXMESH_H

#include <vector>

#include <xkill-utilities/Math.h>

namespace fbxsdk_2014_0_beta2
{
	class FbxNode;
	class FbxMesh;
	class FbxVector2;
	class FbxVector4;

	class FbxLayerElementVertexColor;
	class FbxLayerElementNormal;
	class FbxLayerElementUV;
	class FbxLayerElementTangent;
	class FbxlayerElementBinormal;

	typedef FbxLayerElementVertexColor	FbxGeometryElementVertexColor;
	typedef FbxLayerElementNormal		FbxGeometryElementNormal;
	typedef FbxLayerElementUV			FbxGeometryElementUV;
	typedef FbxLayerElementTangent		FbxGeometryElementTangent;
	typedef FbxLayerElementBinormal		FbxGeometryElementBinormal;
}

class LoaderFbxMeshDesc;

class LoaderFbxMesh
{
public:
	LoaderFbxMesh();
	~LoaderFbxMesh();
	void reset();

	void parseMesh(FbxMesh* mesh, LoaderFbxMeshDesc* meshDesc);
	void createVerticesPosNormTex();
	std::vector<unsigned int> getIndices();
	std::vector<VertexPosNormTex> getVerticesPosNormTex();

	

private:
	void parsePolygonGroup(FbxMesh* mesh, int polygonIndex);
	void parseVertexPositions(FbxMesh* mesh, FbxVector4* controlPoints, int controlPointIndex);
	
	void parseVertexColors(FbxMesh* mesh, int controlPointIndex, int vertexId);
	FbxColor parseVertexColorsByControlPoint(FbxGeometryElementVertexColor* vertexColorElement, int controlPointIndex);
	FbxColor parseVertexColorsByPolygonVertex(FbxGeometryElementVertexColor* vertexColorElement, int vertexId);
	
	void parseVertexNormals(FbxMesh* mesh, int controlPointIndex, int vertexId);
	FbxVector4 parseVertexNormalsByControlPoint(FbxGeometryElementNormal* normalElement, int controlPointIndex);
	FbxVector4 parseVertexNormalsByPolygonVertex(FbxGeometryElementNormal* normalElement, int vertexId);
	
	void parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex);
	FbxVector2 parseVertexUVsByControlPoint(FbxGeometryElementUV* uvElement, int controlPointIndex);
	FbxVector2 parseVertexUVsByPolygonVertex(FbxMesh* mesh, FbxGeometryElementUV* uvElement, int polygonIndex, int insidePolygonIndex);
	
	void parseVertexTangents(FbxMesh* mesh, int controlPointIndex, int vertexId);
	FbxVector4 parseVertexTangentsByControlPoint(FbxGeometryElementTangent* tangentElement, int controlPointIndex);
	FbxVector4 parseVertexTangentsByPolygonVertex(FbxGeometryElementTangent* tangentElement, int vertexId);

	void parseVertexBinormals(FbxMesh* mesh, int controlPointIndex, int vertexId);
	FbxVector4 parseVertexBinormalsByControlPoint(FbxGeometryElementBinormal* binormalElement, int controlPointIndex);
	FbxVector4 parseVertexBinormalsByPolygonVertex(FbxGeometryElementBinormal* binormalElement, int vertexId);

	bool float2Equal(Float2 f1, Float2 f2);
	bool float3Equal(Float3 f1, Float3 f2);
	bool float4Equal(Float4 f1, Float4 f2);

	std::vector<VertexPosNormTex> assemblePosNormTex();
	void indexPosNormTex(std::vector<VertexPosNormTex> posNormTex);
	
	std::vector<unsigned int>	indices_;
	std::vector<VertexPosNormTex> verticesPosNormTex_;

	std::vector<int>	polygonGroupIds_;
	std::vector<Float3> vertexPositions_;
	std::vector<Float4> vertexColors_;
	std::vector<Float3> vertexNormals_;
	std::vector<Float2> vertexUVs_;
	std::vector<Float4> vertexTangents_;
	std::vector<Float4> vertexBinormals_;
};

#endif