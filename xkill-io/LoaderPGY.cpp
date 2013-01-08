#include "LoaderPGY.h"

#include "SpecsPGY.h"

LoaderPGY::LoaderPGY(
	const std::string filePath, 
	const std::string fileName) : Loader(filePath, fileName)
{
	meshModel_ = nullptr;
}
LoaderPGY::~LoaderPGY()
{
	//Do nothing.
}

bool LoaderPGY::init()
{
	bool sucessfulLoad = true;

	std::string fullPath = getFullPath();
	ifstream_ = std::ifstream(
		fullPath, 
		std::ios::in | std::ios::binary);
	
	if(!ifstream_.is_open())
		sucessfulLoad = false;
	else
	{
		PGYHeader header = loadHeader();
		writeTimeUTC_ = header.writeTime_; //store for later
		if(header.versionNum_ == LOADER_PGY_VERSION)
		{
			meshModel_ = loadPGY(
				header.numMaterials_, 
				header.numVertices_, 
				header.numSubsets_);
		}
		else
		{
			sucessfulLoad = false;
			lastError_ = LOADER_ERROR_PGY_VERSION_NUMBER_MISMATCH;
		}

		ifstream_.close();
	}

	return sucessfulLoad;
}

MeshModel* LoaderPGY::loadPGY()
{
	PGYHeader header = loadHeader();

	std::vector<MeshMaterial> materials;
	materials = loadMaterials(header.numMaterials_);

	MeshGeometry geometry;
	geometry = loadGeometry(
		header.numVertices_,
		header.numSubsets_);

	return new MeshModel(geometry, materials);
}
const PGYHeader LoaderPGY::loadHeader()
{
	PGYHeader header;
	ifstream_.read((char*)&header, PGY_SPECS_SIZE_HEADER);
	return header;
}
const std::vector<MeshMaterial>	LoaderPGY::loadMaterials(const unsigned int numMaterials)
{
	std::vector<MeshMaterial> materials;
	for(unsigned int i = 0; i < numMaterials; i++)
		materials.push_back(loadMaterial());

	return materials;
}
const MeshMaterial LoaderPGY::loadMaterial()
{
	MeshMaterial material;
	ifstream_.read(
		(char*)&material, 
		PGY_SPECS_SIZE_MATERIAL);

	return material;
}
const MeshGeometry LoaderPGY::loadGeometry(
	const unsigned int			numVertices,
	const unsigned int			numSubsets)
{
	std::vector<VertexPosNormTex> vertices;
	vertices = loadVertices(numVertices);

	std::vector<MeshSubset> subsets;
	subsets = loadSubsets(numSubsets);

	return MeshGeometry(vertices, subsets);
}
const std::vector<VertexPosNormTex>	LoaderPGY::loadVertices(const unsigned int numVertices)
{
	std::vector<VertexPosNormTex> vertices;
	for(unsigned int i = 0; i < numVertices; i++)
		vertices.push_back(loadVertex());

	return vertices;
}
const VertexPosNormTex LoaderPGY::loadVertex()
{
	VertexPosNormTex vertex;
	ifstream_.read(
		(char*)&vertex, 
		PGY_SPECS_SIZE_VERTEXPOSNORMTEX);

	return vertex;
}
const std::vector<MeshSubset> LoaderPGY::loadSubsets(const unsigned int	numSubsets)
{
	std::vector<MeshSubset> subsets;
	for(unsigned int i = 0; i < numSubsets; i++)
		subsets.push_back(loadSubset());

	return subsets;
}
const MeshSubset LoaderPGY::loadSubset()
{
	/*Load PGY subset header*/
	PGYHeaderSubset ssHeader;
	ifstream_.read(
		(char*)&ssHeader, 
		PGY_SPECS_SIZE_HEADER_SUBSET);
	unsigned int numIndices = ssHeader.numIndices_;

	/*Load MaterialIndex of subset*/
	unsigned int materialIndex;
	ifstream_.read(
		(char*)&materialIndex, 
		sizeof(unsigned int));

	/*Load Indices based on PGY Subset*/
	std::vector<unsigned int> indices;
	unsigned int index;
	for(unsigned int i = 0; i < numIndices; i++)
	{
		ifstream_.read(
		(char*)&index, 
		sizeof(unsigned int));
		indices.push_back(index);
	}

	return MeshSubset(materialIndex, indices);
}

WriteTimeUTC LoaderPGY::getWriteTimeUTC() const
{
	return writeTimeUTC_;
}
MeshModel* LoaderPGY::getMeshModel() const
{
	return meshModel_;
}