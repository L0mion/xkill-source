#ifndef XKILL_IO_LOADERFBXMESHDESC_H
#define XKILL_IO_LOADERFBXMESHDESC_H

#include <vector>

#include <xkill-utilities/Math.h>
#include <xkill-utilities/MeshVertices.h>

#include <fbxsdk.h>

static const unsigned int NUM_INFLUENCING_BONES = 4;


//! Class for describing a loaded mesh
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxMeshDesc
{
public:
	//! Initializes LoaderFbxMeshDesc to its default state.
	LoaderFbxMeshDesc();
	//! Initializes LoaderFbxMeshDesc with specific values.
	LoaderFbxMeshDesc(std::vector<int>	polygonGroupIds,
		std::vector<Float3> vertexPositions,
		std::vector<Float4> vertexColors,
		std::vector<Float3> vertexNormals,
		std::vector<Float2> vertexUVs,
		std::vector<Float4> vertexTangents,
		std::vector<Float4> vertexBinormals);
	//! Currently unused.
	~LoaderFbxMeshDesc();
	//!Resizes vectors used to store bone data.
	/*!
	\param numControlPoints the new size of the vectors.
	*/
	void prepareBoneData(int numControlPoints);
	//!Makes sure vectors for bone data have the required size.
	void fillBoneData();
	//! Adds a bone index to a vertex.
	/*!
	\param vertexIndex Index of the vertex weher the bone index will be added.
	\param boneIndex Index of the bone.
	*/
	void addVertexBoneIndex(unsigned int vertexIndex, int boneIndex);
	//! Adds a weight to a vertex.
	/*!
	\param vertexIndex Index of the vertex weher the bone index will be added.
	\param weight The weight that will be added.
	*/
	void addVertexBoneWeight(unsigned int vertexIndex, float weight);
	//! Adds an offset matrix to the vector storing offset matrices.
	/*!
	\param offsetMatrix The matrix to be added.
	*/
	void addOffsetMatrix(Float4x4 offsetMatrix);

	//! Creates a vector with vertics of type VertexPosColor.
	std::vector<VertexPosColor>				createVerticesPosColor();
	//! Creates a vector with vertics of type VertexPosNormTex. 
	std::vector<VertexPosNormTex>			createVerticesPosNormTex();
	//! Creates a vector with vertics of type VertexPosNormSkinned.
	std::vector<VertexPosNormSkinned>		createVerticesPosNormSkinned();
	//! Creates a vector with vertics of type VertexPosNormTexSkinned.
	std::vector<VertexPosNormTexSkinned>	createVerticesPosNormTexSkinned();
	//! Creates a vector with vertics of type VertexPosNormTexTanSkinned.
	std::vector<VertexPosNormTexTanSkinned> createVerticesPosNormTexTanSkinned();

	void indexData();

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

	//! Assembles vertices of type VertexPosColor.
	std::vector<VertexPosColor>				assembleVertexPosColor();
	//! Assembles vertices of type VertexPosNormTex.
	std::vector<VertexPosNormTex>			assembleVertexPosNormTex();
	//! Assembles vertices of type VertexPosNormSkinned.
	std::vector<VertexPosNormSkinned>		assembleVertexPosNormSkinned();
	//! Assembles vertices of type VertexPosNormTexSkinned.
	std::vector<VertexPosNormTexSkinned>	assembleVertexPosNormTexSkinned();
	//! Assembles vertices of type VertexPosNormTexTanSkinned.
	std::vector<VertexPosNormTexTanSkinned> assembleVertexPosNormTexTanSkinned();

	//! Index vertices of type VertexPosColor, removes doubles and fills the vector indices_ with indices.
	/*!
	\param vertices Vertices to be indexed.
	*/
	std::vector<VertexPosColor>				indexVerticesPosColor(std::vector<VertexPosColor> vertices);
	//! Index vertices of type VertexPosNormTex, removes doubles and fills the vector indices_ with indices.
	/*!
	\param vertices Vertices to be indexed.
	*/
	std::vector<VertexPosNormTex>			indexVerticesPosNormTex(std::vector<VertexPosNormTex> vertices);
	//! Index vertices of type VertexPosNormSkinned, removes doubles and fills the vector indices_ with indices.
	/*!
	\param vertices Vertices to be indexed.
	*/
	std::vector<VertexPosNormSkinned>		indexVerticesPosNormSkinned(std::vector<VertexPosNormSkinned> vertices);
	//! Index vertices of type VertexPosNormTexSkinned, removes doubles and fills the vector indices_ with indices.
	/*!
	\param vertices Vertices to be indexed.
	*/
	std::vector<VertexPosNormTexSkinned>	indexVerticesPosNormTexSkinned(std::vector<VertexPosNormTexSkinned> vertices);
	//! Index vertices of type VertexPosNormTexTanSkinned, removes doubles and fills the vector indices_ with indices.
	/*!
	\param vertices Vertices to be indexed.
	*/
	std::vector<VertexPosNormTexTanSkinned> indexVerticesPosNormTexTanSkinned(std::vector<VertexPosNormTexTanSkinned> vertices);

	//!Checks if two Float2 are equal.
	/*!
	\param Input value 1.
	\param Input value 2.
	*/
	bool float2Equal(Float2 f1, Float2 f2);
	//!Checks if two Float3 are equal.
	/*!
	\param Input value 1.
	\param Input value 2.
	*/
	bool float3Equal(Float3 f1, Float3 f2);
	//!Checks if two Float4 are equal.
	/*!
	\param Input value 1.
	\param Input value 2.
	*/
	bool float4Equal(Float4 f1, Float4 f2);

	std::vector<unsigned int> indices_;			//<! Vector containing indices to a vertex vector.

	std::vector<int>		polygonGroupIds_;	//<! Contains vertex polygon group ids.
	std::vector<Float3>		vertexPositions_;	//<! Contains vertex positions.
	std::vector<Float4>		vertexColors_;		//<! Contains vertex colors.
	std::vector<Float3>		vertexNormals_;		//<! Contains vertex normals.
	std::vector<Float2>		vertexUVs_;			//<! Contains vertex uv-coordinates.
	std::vector<Float4>		vertexTangents_;	//<! Contains vertex tangents.
	std::vector<Float4>		vertexBinormals_;	//<! Contains vertex binormals.

	std::vector<std::vector<int>> vertexBoneIndices_;	//<! Contains vertex bone indices.
	std::vector<std::vector<float>> vertexBoneWeights_;	//<! Contains vertex bone weights.

	std::vector<Float4x4> offsetMatrices_;					//<! Contains offset matrices used to transform vertices into the skeletons lokal space.
	std::vector<int> boneParentIndices_;					//<! Contains the hierarchy of bones.
	std::vector<fbxsdk_2014_0_beta2::FbxNode*> boneNodes_;	//<! Contains the nodes for every bone in the skeleton.
};

#endif //XKILL_IO_LOADERFBXMESHDESC_H