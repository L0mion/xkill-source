#ifndef XKILL_IO_LOADERFBXMESHDESC_H
#define XKILL_IO_LOADERFBXMESHDESC_H

#include <vector>

#include <xkill-utilities/Math.h>
#include <xkill-utilities/MeshVertices.h>

static const unsigned int NUM_INFLUENCING_BONES = 4;

namespace fbxsdk_2014_0_beta2
{
	class FbxNode;
}

class LoaderFbxMeshDesc
{
public:
	LoaderFbxMeshDesc();
	LoaderFbxMeshDesc(std::vector<int>	polygonGroupIds,
		std::vector<Float3> vertexPositions,
		std::vector<Float4> vertexColors,
		std::vector<Float3> vertexNormals,
		std::vector<Float2> vertexUVs,
		std::vector<Float4> vertexTangents,
		std::vector<Float4> vertexBinormals);
	~LoaderFbxMeshDesc();

	void prepareBoneData(int numControlPoints);
	void fillBoneData();
	void addVertexBoneIndex(unsigned int vertexIndex, int boneIndex);
	void addVertexBoneWeight(unsigned int vertexIndex, float weight);
	void addOffsetMatrix(Float4x4 offsetMatrix);

	std::vector<VertexPosColor>				createVerticesPosColor();
	std::vector<VertexPosNormTex>			createVerticesPosNormTex();
	std::vector<VertexPosNormSkinned>		createVerticesPosNormSkinned();
	std::vector<VertexPosNormTexSkinned>	createVerticesPosNormTexSkinned();
	std::vector<VertexPosNormTexTanSkinned> createVerticesPosNormTexTanSkinned();

	std::vector<unsigned int> getIndices();
	
	std::vector<int>	getPolygonGroupIds();	
	std::vector<Float3> getVertexPositions();
	std::vector<Float4> getVertexColors();
	std::vector<Float3>	getVertexNormals();
	std::vector<Float2>	getVertexUVs();
	std::vector<Float4>	getVertexTangents();
	std::vector<Float4>	getVertexBinormals();

	std::vector<std::vector<int>> getVertexBoneIndices();
	std::vector<std::vector<float>> getVertexBoneWeights();
	
	std::vector<Float4x4> getOffsetMatrices();
	std::vector<int> getBoneParentIndices();
	std::vector<FbxNode*> getBoneNodes();

	void setPolygonGroupIds(std::vector<int> polygonGroupIds);
	void setVertexPositions(std::vector<Float3> vertexPositions);
	void setVertexColors(std::vector<Float4> vertexColors);
	void setVertexNormals(std::vector<Float3> vertexNormals);
	void setVertexUVs(std::vector<Float2> vertexUVs);
	void setVertexTangents(std::vector<Float4> vertexTangents);
	void setVertexBinormals(std::vector<Float4> vertexBinormals);
	void setVertexBoneIndices(std::vector<std::vector<int>> vertexBoneIndices);
	void setVertexBoneWeights(std::vector<std::vector<float>> vertexBoneWeights);
	
	void setOffsetMatrices(std::vector<Float4x4> offsetMatrices);
	void setBoneParentIndices(std::vector<int> boneParentIndices);
	void setBoneNodes(std::vector<FbxNode*> boneNodes);

private:

	std::vector<VertexPosColor>				assembleVertexPosColor();
	std::vector<VertexPosNormTex>			assembleVertexPosNormTex();
	std::vector<VertexPosNormSkinned>		assembleVertexPosNormSkinned();
	std::vector<VertexPosNormTexSkinned>	assembleVertexPosNormTexSkinned();
	std::vector<VertexPosNormTexTanSkinned> assembleVertexPosNormTexTanSkinned();

	std::vector<VertexPosColor>				indexVerticesPosColor(std::vector<VertexPosColor> vertices);
	std::vector<VertexPosNormTex>			indexVerticesPosNormTex(std::vector<VertexPosNormTex> vertices);
	std::vector<VertexPosNormSkinned>		indexVerticesPosNormSkinned(std::vector<VertexPosNormSkinned> vertices);
	std::vector<VertexPosNormTexSkinned>	indexVerticesPosNormTexSkinned(std::vector<VertexPosNormTexSkinned> vertices);
	std::vector<VertexPosNormTexTanSkinned> indexVerticesPosNormTexTanSkinned(std::vector<VertexPosNormTexTanSkinned> vertices);

	bool float2Equal(Float2 f1, Float2 f2);
	bool float3Equal(Float3 f1, Float3 f2);
	bool float4Equal(Float4 f1, Float4 f2);

	std::vector<unsigned int> indices_;

	std::vector<int>		polygonGroupIds_;
	std::vector<Float3>		vertexPositions_;
	std::vector<Float4>		vertexColors_;
	std::vector<Float3>		vertexNormals_;
	std::vector<Float2>		vertexUVs_;
	std::vector<Float4>		vertexTangents_;
	std::vector<Float4>		vertexBinormals_;

	std::vector<std::vector<int>> vertexBoneIndices_;
	std::vector<std::vector<float>> vertexBoneWeights_;

	std::vector<Float4x4> offsetMatrices_;
	std::vector<int> boneParentIndices_;
	std::vector<FbxNode*> boneNodes_;
};

#endif //XKILL_IO_LOADERFBXMESHDESC_H