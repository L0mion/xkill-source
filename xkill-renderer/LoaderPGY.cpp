#include "LoaderPGY.h"

LoaderPGY::LoaderPGY(
	const std::string filePath, 
	const std::string fileName) : Loader(filePath, fileName)
{

}
LoaderPGY::~LoaderPGY()
{
	//Do nothing.
}

void LoaderPGY::init()
{
	std::string fullPath = getFullPath();

	ifstream_ = std::ifstream(
		fullPath, 
		std::ios::in | std::ios::binary);

	//TestObj obj;
	//TestVertex v;
	//
	//file.read((char*)&v, sizeof(TestVertex));
	//
	//obj.v = v;
}