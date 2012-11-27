#include <MeshLoader.h>

MeshLoader::MeshLoader(const LPCWSTR mlFileName)
{
	mlFileName_ = mlFileName;
}
MeshLoader::~MeshLoader()
{
	if(mlIFS_.is_open())
		mlIFS_.close();
}

const LPCWSTR MeshLoader::getMLFileName()
{
	return mlFileName_;
}