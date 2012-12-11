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

		materials[i].diffuseMapName.resize(diffuseMapName.size(), ' ');
		materials[i].normalMapName.resize(normalMapName.size(), ' ');

		std::copy(diffuseMapName.begin(), diffuseMapName.end(), materials[i].diffuseMapName.begin());
		std::copy(normalMapName.begin(), normalMapName.end(), materials[i].normalMapName.begin());
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
		infile >> ignore >> vertices[i].tangent_.x >> vertices[i].tangent_.y >> vertices[i].tangent_.z;
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