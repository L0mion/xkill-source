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

void LoaderFbxMeshDesc::prepareBoneData()
{
	unsigned int size = vertexPositions_.size();
	vertexBoneIndices_.resize(size);
	vertexBoneWeights_.resize(size);

	for(unsigned int i=0; i<size; i++)
	{
		vertexBoneIndices_[i].resize(NUM_INFLUENCING_BONES, 0);
		vertexBoneWeights_[i].resize(NUM_INFLUENCING_BONES, 0.0f);
	}
}
void LoaderFbxMeshDesc::addVertexBoneIndex(unsigned int vertexIndex, int boneIndex)
{
	if(vertexIndex < vertexBoneIndices_.size())
	{
		if(vertexBoneIndices_[vertexIndex].size < NUM_INFLUENCING_BONES)
			vertexBoneIndices_[vertexIndex].push_back(boneIndex);
		else
			printf("LoaderFbxMeshDesc::addVertexBoneIndex already at max num bone indices");
	}
	else
		printf("LoaderFbxMeshDesc::addVertexBoneIndex invalid vertex index");
}
void LoaderFbxMeshDesc::addVertexBoneWeight(unsigned int vertexIndex, float weight)
{
	if(vertexIndex < vertexBoneWeights_.size())
	{
		if(vertexBoneWeights_[vertexIndex].size < NUM_INFLUENCING_BONES)
			vertexBoneWeights_[vertexIndex].push_back(weight);
		else
			printf("LoaderFbxMeshDesc::addVertexBoneWeight already at max num bone weights");
	}
	else
		printf("LoaderFbxMeshDesc::addVertexBoneWeight invalid vertex index");
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
void LoaderFbxMeshDesc::setVertexBoneIndices(std::vector<std::vector<int>> vertexBoneIndices)
{
	vertexBoneIndices_ = vertexBoneIndices;
}
void LoaderFbxMeshDesc::setVertexBoneWeights(std::vector<std::vector<float>> vertexBoneWeights)
{
	vertexBoneWeights_ = vertexBoneWeights;
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

std::vector<std::vector<int>> LoaderFbxMeshDesc::getVertexBoneIndices()
{
	return vertexBoneIndices_;
}
std::vector<std::vector<float>> LoaderFbxMeshDesc::getVertexBoneWeights()
{
	return vertexBoneWeights_;
}