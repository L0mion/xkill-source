#include <MeshLoader.h>

MeshLoader::MeshLoader(const LPCWSTR mlFileName)
{
	mlFilePath_ = mlFileName;
}
MeshLoader::~MeshLoader()
{
	if(mlIFS_.is_open())
		mlIFS_.close();
}

const LPCWSTR MeshLoader::getMLFilePath()
{
	return mlFilePath_;
}