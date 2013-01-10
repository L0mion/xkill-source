#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>

#include <fbxsdk.h>
#include <fbxsdk/fbxsdk_compatibility.h>

static const unsigned int POLYGON_SIZE = 3;

class MaterialDescFbx;
struct VertexPosNormTex;

class LoaderFbx
{
public:
	LoaderFbx();
	~LoaderFbx();

	bool load(std::string filename);
	
	void createVerteciesPosNormTex();

	std::vector<unsigned int> getIndices();
	std::vector<VertexPosNormTex> getVerticesPosNormTex();

private:
	bool createFbxManager();
	bool createFbxScene();
	bool loadScene(std::string filename);
	
	void parseNode(FbxNode* node);
	void parseMesh(FbxNode* node);

	void parseVertexPositions(FbxMesh* mesh, int polygonVertexCount);
	void parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId);
	void parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex);
	void parseVertexTangents(FbxMesh* mesh, int vertexId);
	void parseVertexBinormals(FbxMesh* mesh, int vertexId);

	void parseMaterial(FbxGeometry* geometry);
	void parseMaterialImplementation(FbxSurfaceMaterial* material, const FbxImplementation* implementation, int materialIndex, MaterialDescFbx* materialDesc);
	void parseMaterialPhong(FbxSurfaceMaterial* material, int materialIndex, MaterialDescFbx* materialDesc);
	void parseMaterialLambert(FbxSurfaceMaterial* material, int materialIndex, MaterialDescFbx* materialDesc);
	void parseMaterialUnknown();

	void extractPhongAmbient(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongDiffuse(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongSpecular(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongEmissive(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongOpacity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongShininess(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractPhongReflectivity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);

	void extractLambertAmbient(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractLambertDiffuse(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractLambertEmissive(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);
	void extractLambertOpacity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc);

	void parseAnimation(FbxScene* scene);
	void parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher);
	void parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher);
	void parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher);

	void clearPreviousGeometryData();

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

	FbxManager*		fbxManager_;
	FbxScene*		fbxScene_;
};

#endif