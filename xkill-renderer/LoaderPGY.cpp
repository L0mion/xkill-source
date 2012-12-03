#include "LoaderPGY.h"

#include "SpecsPGY.h"

LoaderPGY::LoaderPGY(
	const std::string filePath, 
	const std::string fileName) : Loader(filePath, fileName)
{

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
		meshModel_ = loadPGY();

		ifstream_.close();
	}

	

	//TestObj obj;
	//TestVertex v;
	//
	//file.read((char*)&v, sizeof(TestVertex));
	//
	//obj.v = v;

	return true;
}

const MeshModel LoaderPGY::loadPGY()
{
	/*Load PGY header*/
	PGYHeader header = loadHeader();

	/*Load PGY materials*/
	std::vector<MeshMaterial> materials;
	unsigned int numMaterials = header.materialsNum;
	for(unsigned int i = 0; i < numMaterials; i++)
		materials.push_back(loadMaterial());

	MeshGeometry temp;

	MeshModel model(temp, materials);
	return model;
}
const PGYHeader LoaderPGY::loadHeader()
{
	PGYHeader header;
	ifstream_.read((char*)&header, PGY_SPECS_HEADER_SIZE);
	return header;
}
const MeshMaterial LoaderPGY::loadMaterial()
{
	MeshMaterial material;
	ifstream_.read((char*)&material, PGY_SPECS_MATERIAL_SIZE);
	return material;
}

const MeshModel LoaderPGY::getMeshModel()
{
	return meshModel_;
}