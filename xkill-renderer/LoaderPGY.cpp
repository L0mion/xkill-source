#include "LoaderPGY.h"

#include "PGYFormat.h"

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
		PGYHeader header = loadHeader();

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

const PGYHeader LoaderPGY::loadHeader()
{
	PGYHeader header;
	ifstream_.read((char*)&header, sizeof(header));
	return header;
}

const MeshModel LoaderPGY::getMeshModel()
{
	return meshModel_;
}