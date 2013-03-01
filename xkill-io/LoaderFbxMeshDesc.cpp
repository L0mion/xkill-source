#include <fbxsdk.h>

#include <xkill-utilities/MeshVertices.h>

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

void LoaderFbxMeshDesc::prepareBoneData(int numControlPoints)
{
	vertexBoneIndices_.resize(numControlPoints);
	vertexBoneWeights_.resize(numControlPoints);
}
void LoaderFbxMeshDesc::fillBoneData()
{
	for(unsigned int i=0; i<vertexBoneIndices_.size(); i++)
	{
		while(vertexBoneIndices_[i].size() < NUM_BONES_PER_VERTEX)
			vertexBoneIndices_[i].push_back(0);
		while(vertexBoneWeights_[i].size() < NUM_BONES_PER_VERTEX)
			vertexBoneWeights_[i].push_back(0.0f);
	}
}
void LoaderFbxMeshDesc::addVertexBoneIndex(unsigned int vertexIndex, int boneIndex)
{
	if(vertexIndex < vertexBoneIndices_.size())
	{
		if(vertexBoneIndices_[vertexIndex].size() < NUM_BONES_PER_VERTEX)
			vertexBoneIndices_[vertexIndex].push_back(boneIndex);
		//else
		//	printf("LoaderFbxMeshDesc::addVertexBoneIndex already at max num bone indices \n");
	}
	//else
	//	printf("LoaderFbxMeshDesc::addVertexBoneIndex invalid vertex index \n");
}
void LoaderFbxMeshDesc::addVertexBoneWeight(unsigned int vertexIndex, float weight)
{
	if(vertexIndex < vertexBoneWeights_.size())
	{
		if(vertexBoneWeights_[vertexIndex].size() < NUM_INFLUENCING_BONES)
			vertexBoneWeights_[vertexIndex].push_back(weight);
		//else
		//	printf("LoaderFbxMeshDesc::addVertexBoneWeight already at max num bone weights \n");
	}
	//else
	//	printf("LoaderFbxMeshDesc::addVertexBoneWeight invalid vertex index \n");
}
void LoaderFbxMeshDesc::addOffsetMatrix(Float4x4 offsetMatrix)
{
	offsetMatrices_.push_back(offsetMatrix);
}

void LoaderFbxMeshDesc::createVertices(std::vector<VertexDesc>& vertices, std::vector<unsigned int>& indexBuffer)
{
//	std::vector<VertexDesc> vertices;
	vertices = assembleVertexDesc();
	vertices = indexVertexDesc(vertices);
	indexBuffer = indices_;

	if(vertexBoneIndices_.size() > 0 && vertexBoneWeights_.size() > 0)
	{
		std::vector<unsigned int> indices;
		indices = calculateBoneMappingIndices(vertices);
		mapBoneData(&vertices, indices);
	}

	//return vertices;
}
std::vector<VertexDesc> LoaderFbxMeshDesc::assembleVertexDesc()
{
	std::vector<VertexDesc> vertices;

	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexDesc vertex;
		
		vertex.position_ = vertexPositions_[i];
		if(i<vertexNormals_.size())
			vertex.normal_ = vertexNormals_[i];
		if(i<vertexUVs_.size())
			vertex.textureCoordinates_ = vertexUVs_[i];
		if(i<vertexTangents_.size())
			vertex.tangent_ = vertexTangents_[i];

		vertices.push_back(vertex);
	}
	return vertices;
}
std::vector<VertexDesc> LoaderFbxMeshDesc::indexVertexDesc(std::vector<VertexDesc> vertices)
{
	indices_.clear();
	std::vector<VertexDesc> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			if(vertexPositions_.size() > 0)
				equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(vertexNormals_.size() > 0)
			{
				if(equal)
					equal = float3Equal(vertices[i].normal_, indexedVertices[index].normal_);
			}
			if(vertexUVs_.size() > 0)
			{
				if(equal)
					equal = float2Equal(vertices[i].textureCoordinates_, indexedVertices[index].textureCoordinates_);
			}
			if(vertexTangents_.size() > 0)
			{
				if(equal)
					equal = float4Equal(vertices[i].tangent_, indexedVertices[index].tangent_);
			}
			if(!equal)
				index++;
		}
		if(equal)
			indices_.push_back(index);
		else
		{
			indexedVertices.push_back(vertices[i]);
			indices_.push_back(indexedVertices.size()-1);
		}
	}
	return indexedVertices;
}
std::vector<unsigned int> LoaderFbxMeshDesc::calculateBoneMappingIndices(std::vector<VertexDesc> vertices)
{
	std::vector<VertexDesc> indexedVertices;
	std::vector<unsigned int> indices;
	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal = false;
		unsigned int index = 0;
		while(index < indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(!equal)
				index++;
		}
		if(equal)
			indices.push_back(index);
		else
		{
			indexedVertices.push_back(vertices[i]);
			indices.push_back(indexedVertices.size()-1);
		}
	}
	return indices;
}
void LoaderFbxMeshDesc::mapBoneData(std::vector<VertexDesc>* vertices, std::vector<unsigned int> indices)
{
//	for(unsigned int i=0; i<vertices->size(); i++)
//	{
//		vertices->at(i).weights_.x = vertexBoneWeights_[indices[i]][0];
//		vertices->at(i).weights_.y = vertexBoneWeights_[indices[i]][1];
//		vertices->at(i).weights_.z = vertexBoneWeights_[indices[i]][2];
//		for(int boneIndex=0; boneIndex<NUM_BONES_PER_VERTEX; boneIndex++)
//		{
//			vertices->at(i).boneIndices_[boneIndex] = vertexBoneIndices_[indices[i]][boneIndex];
//		}
//	}



	int numControlPoints = fbxMesh_->GetControlPointsCount();
	FbxVector4* controlPoints = fbxMesh_->GetControlPoints();
	for(unsigned int vertexIndex = 0; vertexIndex<vertices->size(); vertexIndex++)
	{
		for(int controlPointIndex=0; controlPointIndex<numControlPoints; controlPointIndex++)
		{
			Float3 controlPoint;
			controlPoint.x = static_cast<float>(controlPoints[controlPointIndex].mData[0]);
			controlPoint.y = static_cast<float>(controlPoints[controlPointIndex].mData[1]);
			controlPoint.z = static_cast<float>(controlPoints[controlPointIndex].mData[2]);

			if(float3Equal(controlPoint, vertices->at(vertexIndex).position_))
			{
				vertices->at(vertexIndex).weights_.x = vertexBoneWeights_[controlPointIndex][0];
				vertices->at(vertexIndex).weights_.y = vertexBoneWeights_[controlPointIndex][1];
				vertices->at(vertexIndex).weights_.z = vertexBoneWeights_[controlPointIndex][2];
				for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
					vertices->at(vertexIndex).boneIndices_[i] = vertexBoneIndices_[controlPointIndex][i];

				break; // <-- FIX THIS SHIT!
			}
		}
	}
}

bool LoaderFbxMeshDesc::float2Equal(Float2 f1, Float2 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y)
		equal = true;

	return equal;
}
bool LoaderFbxMeshDesc::float3Equal(Float3 f1, Float3 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z)
		equal = true;

	return equal;
}
bool LoaderFbxMeshDesc::float4Equal(Float4 f1, Float4 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z && f1.w == f2.w)
		equal = true;

	return equal;
}

std::vector<unsigned int> LoaderFbxMeshDesc::getIndices()
{
	return indices_;
}
std::vector<int>		  LoaderFbxMeshDesc::getPolygonGroupIds()
{
	return polygonGroupIds_;
}
std::vector<Float3>		  LoaderFbxMeshDesc::getVertexPositions()
{
	return vertexPositions_;
}
std::vector<Float4>		  LoaderFbxMeshDesc::getVertexColors()
{
	return vertexColors_;
}
std::vector<Float3>		  LoaderFbxMeshDesc::getVertexNormals()
{
	return vertexNormals_;
}
std::vector<Float2>		  LoaderFbxMeshDesc::getVertexUVs()
{
	return vertexUVs_;
}
std::vector<Float4>		  LoaderFbxMeshDesc::getVertexTangents()
{
	return vertexTangents_;
}
std::vector<Float4>		  LoaderFbxMeshDesc::getVertexBinormals()
{
	return vertexBinormals_;
}

std::vector<std::vector<int>>	LoaderFbxMeshDesc::getVertexBoneIndices()
{
	return vertexBoneIndices_;
}
std::vector<std::vector<float>> LoaderFbxMeshDesc::getVertexBoneWeights()
{
	return vertexBoneWeights_;
}

std::vector<Float4x4> LoaderFbxMeshDesc::getOffsetMatrices()
{
	return offsetMatrices_;
}
std::vector<int>	  LoaderFbxMeshDesc::getBoneParentIndices()
{
	return boneParentIndices_;
}
std::vector<FbxNode*> LoaderFbxMeshDesc::getBoneNodes()
{
	return boneNodes_;
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

void LoaderFbxMeshDesc::setOffsetMatrix(unsigned int index, Float4x4 offsetMatrix)
{
	if(index < offsetMatrices_.size())
		offsetMatrices_[index] = offsetMatrix;
}
void LoaderFbxMeshDesc::setOffsetMatrices(std::vector<Float4x4> offsetMatrices)
{
	offsetMatrices_ = offsetMatrices;
}
void LoaderFbxMeshDesc::setBoneParentIndices(std::vector<int> boneParentIndices)
{
	boneParentIndices_ = boneParentIndices;
}
void LoaderFbxMeshDesc::setBoneNodes(std::vector<FbxNode*> boneNodes)
{
	boneNodes_ = boneNodes;
}

void LoaderFbxMeshDesc::setFbxMesh(FbxMesh* fbxMesh)
{
	fbxMesh_ = fbxMesh;
}