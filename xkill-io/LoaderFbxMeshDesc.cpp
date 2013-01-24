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
		if(vertexBoneIndices_[vertexIndex].size() < NUM_INFLUENCING_BONES)
			vertexBoneIndices_[vertexIndex].push_back(boneIndex);
		else
			printf("LoaderFbxMeshDesc::addVertexBoneIndex already at max num bone indices \n");
	}
	else
		printf("LoaderFbxMeshDesc::addVertexBoneIndex invalid vertex index \n");
}
void LoaderFbxMeshDesc::addVertexBoneWeight(unsigned int vertexIndex, float weight)
{
	if(vertexIndex < vertexBoneWeights_.size())
	{
		if(vertexBoneWeights_[vertexIndex].size() < NUM_INFLUENCING_BONES)
			vertexBoneWeights_[vertexIndex].push_back(weight);
		else
			printf("LoaderFbxMeshDesc::addVertexBoneWeight already at max num bone weights \n");
	}
	else
		printf("LoaderFbxMeshDesc::addVertexBoneWeight invalid vertex index \n");
}
void LoaderFbxMeshDesc::addOffsetMatrix(Float4x4 offsetMatrix)
{
	offsetMatrices_.push_back(offsetMatrix);
}


std::vector<VertexPosColor>				LoaderFbxMeshDesc::createVerticesPosColor()
{
	std::vector<VertexPosColor> vertices;
	vertices = assembleVertexPosColor();
	vertices = indexVerticesPosColor(vertices);

	return vertices;
}
std::vector<VertexPosNormTex>			LoaderFbxMeshDesc::createVerticesPosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	vertices = assembleVertexPosNormTex();
	vertices = indexVerticesPosNormTex(vertices);

	return vertices;
}
std::vector<VertexPosNormSkinned>		LoaderFbxMeshDesc::createVerticesPosNormSkinned()
{
	std::vector<VertexPosNormSkinned> vertices;
	vertices = assembleVertexPosNormSkinned();
	vertices = indexVerticesPosNormSkinned(vertices);

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		vertices[i].weights_.x = vertexBoneWeights_[i][0];
		vertices[i].weights_.y = vertexBoneWeights_[i][1];
		vertices[i].weights_.z = vertexBoneWeights_[i][2];
		for(int boneIndex=0; boneIndex<NUM_BONES_PER_VERTEX; boneIndex++)
		{
			vertices[i].boneIndices_[boneIndex] = vertexBoneIndices_[i][boneIndex];
		}
	}

	return vertices;
}
std::vector<VertexPosNormTexSkinned>	LoaderFbxMeshDesc::createVerticesPosNormTexSkinned()
{
	std::vector<VertexPosNormTexSkinned> vertices;
	vertices = assembleVertexPosNormTexSkinned();
	vertices = indexVerticesPosNormTexSkinned(vertices);

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		vertices[i].weights_.x = vertexBoneWeights_[i][0];
		vertices[i].weights_.y = vertexBoneWeights_[i][1];
		vertices[i].weights_.z = vertexBoneWeights_[i][2];
		for(int boneIndex=0; i<NUM_BONES_PER_VERTEX; i++)
		{		
			vertices[i].boneIndices_[boneIndex] = vertexBoneIndices_[i][boneIndex];
		}
	}

	return vertices;
}
std::vector<VertexPosNormTexTanSkinned> LoaderFbxMeshDesc::createVerticesPosNormTexTanSkinned()
{
	std::vector<VertexPosNormTexTanSkinned> vertices;
	vertices = assembleVertexPosNormTexTanSkinned();
	vertices = indexVerticesPosNormTexTanSkinned(vertices);

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		vertices[i].weights_.x = vertexBoneWeights_[i][0];
		vertices[i].weights_.y = vertexBoneWeights_[i][1];
		vertices[i].weights_.z = vertexBoneWeights_[i][2];
		for(int boneIndex=0; i<NUM_BONES_PER_VERTEX; i++)
		{
			vertices[i].boneIndices_[boneIndex] = vertexBoneIndices_[i][boneIndex];
		}
	}

	return vertices;
}

std::vector<VertexPosColor>				LoaderFbxMeshDesc::assembleVertexPosColor()
{
	std::vector<VertexPosColor> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosColor vertex;
		vertex.position_ = vertexPositions_[i];
		vertex.color_.x	 = vertexColors_[i].x;
		vertex.color_.y	 = vertexColors_[i].y;
		vertex.color_.z	 = vertexColors_[i].z;

		vertices.push_back(vertex);
	}
	return vertices;
}
std::vector<VertexPosNormTex>			LoaderFbxMeshDesc::assembleVertexPosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormTex vertex;
		vertex.position_ = vertexPositions_[i];
		vertex.normal_	 = vertexNormals_[i];
		vertex.texcoord_ = vertexUVs_[i];

		vertices.push_back(vertex);
	}
	return vertices;
}
std::vector<VertexPosNormSkinned>		LoaderFbxMeshDesc::assembleVertexPosNormSkinned()
{
	std::vector<VertexPosNormSkinned> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormSkinned vertex;
		vertex.position_ = vertexPositions_[i];
		vertex.normal_	 = vertexNormals_[i];

		//Bone indices and weights are already in an indexed format and
		//will therefor not be assembled into the complete vertex here.

		vertices.push_back(vertex);
	}
	return vertices;
}
std::vector<VertexPosNormTexSkinned>	LoaderFbxMeshDesc::assembleVertexPosNormTexSkinned()
{
	std::vector<VertexPosNormTexSkinned> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormTexSkinned vertex;
		vertex.position_ = vertexPositions_[i];
		vertex.normal_	 = vertexNormals_[i];
		vertex.texcoord_ = vertexUVs_[i];

		//Bone indices and weights are already in an indexed format and
		//will therefor not be assembled into the complete vertex here.

		vertices.push_back(vertex);
	}
	return vertices;
}
std::vector<VertexPosNormTexTanSkinned> LoaderFbxMeshDesc::assembleVertexPosNormTexTanSkinned()
{
	std::vector<VertexPosNormTexTanSkinned> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormTexTanSkinned vertex;
		vertex.position_ = vertexPositions_[i];
		vertex.normal_	 = vertexNormals_[i];
		vertex.texcoord_ = vertexUVs_[i];
		vertex.tangent_  = vertexTangents_[i];

		//Bone indices and weights are already in an indexed format and
		//will therefor not be assembled into the complete vertex here.

		vertices.push_back(vertex);
	}
	return vertices;
}

std::vector<VertexPosColor>				LoaderFbxMeshDesc::indexVerticesPosColor(std::vector<VertexPosColor> vertices)
{
	indices_.clear();
	std::vector<VertexPosColor> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(equal)
				equal = float3Equal(vertices[i].color_, indexedVertices[index].color_);
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
std::vector<VertexPosNormTex>			LoaderFbxMeshDesc::indexVerticesPosNormTex(std::vector<VertexPosNormTex> vertices)
{
	indices_.clear();
	std::vector<VertexPosNormTex> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(equal)
				equal = float3Equal(vertices[i].normal_, indexedVertices[index].normal_);
			if(equal)
				equal = float2Equal(vertices[i].texcoord_, indexedVertices[index].texcoord_);
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
std::vector<VertexPosNormSkinned>		LoaderFbxMeshDesc::indexVerticesPosNormSkinned(std::vector<VertexPosNormSkinned> vertices)
{
	indices_.clear();
	std::vector<VertexPosNormSkinned> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(equal)
				equal = float3Equal(vertices[i].normal_, indexedVertices[index].normal_);
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
std::vector<VertexPosNormTexSkinned>	LoaderFbxMeshDesc::indexVerticesPosNormTexSkinned(std::vector<VertexPosNormTexSkinned> vertices)
{
	indices_.clear();
	std::vector<VertexPosNormTexSkinned> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(equal)
				equal = float3Equal(vertices[i].normal_, indexedVertices[index].normal_);
			if(equal)
				equal = float2Equal(vertices[i].texcoord_, indexedVertices[index].texcoord_);
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
std::vector<VertexPosNormTexTanSkinned> LoaderFbxMeshDesc::indexVerticesPosNormTexTanSkinned(std::vector<VertexPosNormTexTanSkinned> vertices)
{
	indices_.clear();
	std::vector<VertexPosNormTexTanSkinned> indexedVertices;

	for(unsigned int i=0; i<vertices.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<indexedVertices.size() && !equal)
		{
			equal = float3Equal(vertices[i].position_, indexedVertices[index].position_);
			if(equal)
				equal = float3Equal(vertices[i].normal_, indexedVertices[index].normal_);
			if(equal)
				equal = float2Equal(vertices[i].texcoord_, indexedVertices[index].texcoord_);
			if(equal)
				equal = float4Equal(vertices[i].tangent_, indexedVertices[index].tangent_);
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

std::vector<std::vector<int>> LoaderFbxMeshDesc::getVertexBoneIndices()
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
std::vector<int> LoaderFbxMeshDesc::getBoneParentIndices()
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