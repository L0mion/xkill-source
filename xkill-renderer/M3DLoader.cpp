#include "M3DLoader.h"

M3DLoader::M3DLoader()
{
}

M3DLoader::~M3DLoader()
{
}

void M3DLoader::loadM3D(const std::string& filename,
						std::vector<VertexPosNormTexTanSkinned>& vertices,
						std::vector<unsigned int>& indices,
						std::vector<M3DSubset>& subsets,
						std::vector<M3DMaterial>& materials,
						SkinnedData& skinInfo)
{
	std::ifstream infile(filename);

	unsigned int numMaterials		= 0;
	unsigned int numVertices		= 0;
	unsigned int numTriangles		= 0;
	unsigned int numBones			= 0;
	unsigned int numAnimationClips	= 0;

	std::string ignore;

	if(infile)
	{
		infile >> ignore; //Header text;
		infile >> ignore >> numMaterials;
		infile >> ignore >> numVertices;
		infile >> ignore >> numTriangles;
		infile >> ignore >> numBones;
		infile >> ignore >> numAnimationClips;

		std::vector<DirectX::XMFLOAT4X4> boneOffsets;
		std::vector<int> boneIndexToParentIndex;
		std::map<std::string, AnimationClip> animations;

		parseMaterials(infile, numMaterials, materials);
		parseSubsetTable(infile, numMaterials, subsets);
		parseSkinnedVertices(infile, numVertices, vertices);
		parseTriangles(infile, numTriangles, indices);
		parseBoneOffsets(infile, numBones, boneOffsets);
		parseBoneHierarchy(infile, numBones, boneIndexToParentIndex);
		parseAnimationClips(infile, numAnimationClips, numBones, animations);

		skinInfo.init(boneIndexToParentIndex, boneOffsets, animations);
	}
}

void M3DLoader::parseMaterials(std::ifstream& infile, unsigned int numMaterials, std::vector<M3DMaterial>& materials)
{
	std::string ignore;

	materials.resize(numMaterials);

	std::string diffuseMapName;
	std::string normalMapName;

	infile >> ignore; //Header text;
	for(unsigned int i=0; i<numMaterials; i++)
	{
		infile >> ignore >> materials[i].ambient_.x >> materials[i].ambient_.y >> materials[i].ambient_.z;
		infile >> ignore >> materials[i].diffuse_.x >> materials[i].diffuse_.y >> materials[i].diffuse_.z;
		infile >> ignore >> materials[i].specular_.x >> materials[i].specular_.y >> materials[i].specular_.z;
		infile >> ignore >> materials[i].specular_.w;
		infile >> ignore >> materials[i].reflectivity_.x >> materials[i].reflectivity_.y >> materials[i].reflectivity_.z;
		infile >> ignore >> materials[i].alphaClip_;
		infile >> ignore >> materials[i].effectTypeName;
		infile >> ignore >> diffuseMapName;
		infile >> ignore >> normalMapName;

		materials[i].diffuseMapName = diffuseMapName;
		materials[i].normalMapName = normalMapName;
	}
}

void M3DLoader::parseSubsetTable(std::ifstream& infile, unsigned int numSubsets, std::vector<M3DSubset>& subsets)
{
	std::string ignore;
	subsets.resize(numSubsets);

	infile >> ignore; //Header text
	for(unsigned int i=0; i<numSubsets; i++)
	{
		infile >> ignore >> subsets[i].id_;
		infile >> ignore >> subsets[i].vertexStart_;
		infile >> ignore >> subsets[i].vertexCount_;
		infile >> ignore >> subsets[i].faceStart_;
		infile >> ignore >> subsets[i].faceCount_;
	}
}

void M3DLoader::parseSkinnedVertices(std::ifstream& infile, unsigned int numVertices, std::vector<VertexPosNormTexTanSkinned>& vertices)
{
	std::string ignore;
	vertices.resize(numVertices);

	infile >> ignore; //Header text.
	
	int		boneIndices[4];
	float	weights[4];
	
	for(unsigned int i=0; i<numVertices; i++)
	{
		infile >> ignore >> vertices[i].position_.x >> vertices[i].position_.y >> vertices[i].position_.z;
		infile >> ignore >> vertices[i].tangent_.x >> vertices[i].tangent_.y >> vertices[i].tangent_.z >> vertices[i].tangent_.w;
		infile >> ignore >> vertices[i].normal_.x >> vertices[i].normal_.y >> vertices[i].normal_.z;
		infile >> ignore >> vertices[i].texcoord_.x >> vertices[i].texcoord_.y;
		infile >> ignore >> weights[0] >> weights[1] >> weights[2] >> weights[3];
		infile >> ignore >> boneIndices[0] >> boneIndices[1] >> boneIndices[2] >> boneIndices[3];

		vertices[i].weights_.x = weights[0];
		vertices[i].weights_.y = weights[1];
		vertices[i].weights_.z = weights[2];

		vertices[i].boneIndices_[0] = boneIndices[0]; 
		vertices[i].boneIndices_[1] = boneIndices[1]; 
		vertices[i].boneIndices_[2] = boneIndices[2]; 
		vertices[i].boneIndices_[3] = boneIndices[3]; 
	}
}

void M3DLoader::parseTriangles(std::ifstream& infile, unsigned int numTriangles, std::vector<unsigned int>& indices)
{
	std::string ignore;
	indices.resize(numTriangles*3);

	infile >> ignore; //Header text
	for(unsigned int i=0; i<numTriangles; i++)
	{
		infile >> indices[i*3+0] >> indices[i*3+1] >> indices[i*3+2];
	}
}

void M3DLoader::parseBoneOffsets(std::ifstream& infile, unsigned int numBones, std::vector<DirectX::XMFLOAT4X4>& boneOffsets)
{
	std::string ignore;
	boneOffsets.resize(numBones);

	infile >> ignore; //Header text
	for(unsigned int i=0; i<numBones; i++)
	{
		infile >> ignore >> boneOffsets[i](0,0) >> boneOffsets[i](0,1) >> boneOffsets[i](0,2) >> boneOffsets[i](0,3) >>
							boneOffsets[i](1,0) >> boneOffsets[i](1,1) >> boneOffsets[i](1,2) >> boneOffsets[i](1,3) >>
							boneOffsets[i](2,0) >> boneOffsets[i](2,1) >> boneOffsets[i](2,2) >> boneOffsets[i](2,3) >>
							boneOffsets[i](3,0) >> boneOffsets[i](3,1) >> boneOffsets[i](3,2) >> boneOffsets[i](3,3);
	}
}

void M3DLoader::parseBoneHierarchy(std::ifstream& infile, unsigned int numBones, std::vector<int>& boneIndexToParentIndex)
{
	std::string ignore;
	boneIndexToParentIndex.resize(numBones);

	infile >> ignore;
	for(unsigned int i=0; i<numBones; i++)
	{
		infile >> ignore >> boneIndexToParentIndex[i];
	}
}

void M3DLoader::parseAnimationClips(std::ifstream& infile, unsigned int numAnimationClips,
									unsigned int numBones, std::map<std::string, AnimationClip>& animations)
{
	std::string ignore;
	infile >> ignore; //Header Text

	for(unsigned int clipIndex = 0; clipIndex < numAnimationClips; clipIndex++)
	{
		std::string clipName;
		infile >> ignore >> clipName;
		infile >> ignore; // {

		AnimationClip clip;
		clip.boneAnimations_.resize(numBones);

		for(unsigned int boneIndex=0; boneIndex<numBones; boneIndex++)
		{
			parseBoneKeyframes(infile, numBones, clip.boneAnimations_.at(boneIndex));
		}
		infile >> ignore; // {

		animations[clipName] = clip;
	}
}

void M3DLoader::parseBoneKeyframes(std::ifstream& infile, unsigned int numBones, BoneAnimation& boneAnimation)
{
	std::string ignore;
	unsigned int numKeyframes = 0;
	infile >> ignore >> ignore >> numKeyframes;
	infile >> ignore; // {
	
	boneAnimation.keyframes_.resize(numKeyframes);
	for(unsigned int i=0; i<numKeyframes; i++)
	{
		float time = 0.0f;
		DirectX::XMFLOAT3 translation(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 scale(1.0f, 1.0f, 1.0f);
		DirectX::XMFLOAT4 rotationQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

		infile >> ignore >> time;
		infile >> ignore >> translation.x >> translation.y >> translation.z;
		infile >> ignore >> scale.x >> scale.y >> scale.z;
		infile >> ignore >> rotationQuaternion.x >> rotationQuaternion.y >> rotationQuaternion.z >> rotationQuaternion.w;

		boneAnimation.keyframes_[i].timePosition		= time;
		boneAnimation.keyframes_[i].translation			= translation;
		boneAnimation.keyframes_[i].scale				= scale;
		boneAnimation.keyframes_[i].rotationQuaternion	= rotationQuaternion;	
	}

	infile >> ignore; // {
}