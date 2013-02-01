#include "LoaderPGY.h"

#include "SpecsPGY.h"

LoaderPGY::LoaderPGY(
	const std::string filePath, 
	const std::string fileName) : Loader(filePath, fileName)
{
}
LoaderPGY::~LoaderPGY()
{
}

bool LoaderPGY::init()
{
	bool sucessfulLoad = true;

	std::string fullPath = getFullPath();
	ifstream_ = std::ifstream(
		fullPath, 
		std::ios::in | std::ios::binary);
	
	if(!ifstream_.is_open())
	{
		sucessfulLoad = false;
		lastError_ = LOADER_ERROR_FILE_NOT_FOUND;
	}
	else
	{
		PGYHeader header = loadHeader();
		writeTimeUTC_ = header.writeTime_; //store for later
		if(header.versionNum_ == LOADER_PGY_VERSION)
		{
			meshDesc_ =  loadPGY(
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

MeshDesc LoaderPGY::getMeshModel()
{
	return meshDesc_;
}

MeshDesc LoaderPGY::loadPGY(
	unsigned int numMaterials,
	unsigned int numVertices,
	unsigned int numSubsets)
{
	std::vector<MaterialDesc> materials;
	materials = loadMaterials(numMaterials);

	std::vector<VertexDesc> vertices;
	std::vector<SubsetDesc> subsets;
	loadGeometry(
		numVertices,
		numSubsets,
		vertices,
		subsets);

	std::string fileName = getFileName();

	MeshDesc meshDesc;
	meshDesc.materials_		= materials;
	meshDesc.vertices_		= vertices;
	meshDesc.subsets_		= subsets;
	return meshDesc;
}
const PGYHeader LoaderPGY::loadHeader()
{
	PGYHeader header;
	ifstream_.read(
		(char*)&header, 
		sizeof(PGYHeader));
	return header;
}
const std::vector<MaterialDesc>	LoaderPGY::loadMaterials(const unsigned int numMaterials)
{
	std::vector<MaterialDesc> materials;
	for(unsigned int i = 0; i < numMaterials; i++)
		materials.push_back(loadMaterial());

	return materials;
}
MaterialDesc LoaderPGY::loadMaterial()
{
	MaterialDesc material;
	ifstream_.read(
		(char*)&material, 
		sizeof(MaterialDesc));

	return material;
}
void LoaderPGY::loadGeometry(
	const unsigned int			numVertices,
	const unsigned int			numSubsets,
	std::vector<VertexDesc>& vertices,
	std::vector<SubsetDesc>& subsets)
{
	loadVertices(numVertices, vertices);
	loadSubsets(numSubsets, subsets);
}
void LoaderPGY::loadVertices(const unsigned int numVertices, std::vector<VertexDesc>& vertices)
{
	for(unsigned int i = 0; i < numVertices; i++)
		vertices.push_back(loadVertex());
}
const VertexDesc LoaderPGY::loadVertex()
{
	VertexDesc vertex;
	ifstream_.read(
		(char*)&vertex, 
		sizeof(VertexDesc));

	return vertex;
}
void LoaderPGY::loadSubsets(const unsigned int	numSubsets, std::vector<SubsetDesc>& subsets)
{
	for(unsigned int i = 0; i < numSubsets; i++)
	{
		subsets.push_back(loadSubset());
	}
}
const SubsetDesc LoaderPGY::loadSubset()
{
	/*Load PGY subset header*/
	PGYHeaderSubset ssHeader;
	ifstream_.read(
		(char*)&ssHeader, 
		sizeof(PGYHeaderSubset));
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

	SubsetDesc subset;
	subset.materialIndex_	= materialIndex;
	subset.indices_			= indices;
	return subset;
}

WriteTimeUTC LoaderPGY::getWriteTimeUTC() const
{
	return writeTimeUTC_;
}