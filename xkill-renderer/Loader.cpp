#include <Loader.h>

Loader::Loader(const LPCWSTR mlFileName)
{
	filePath_ = mlFileName;
}
Loader::~Loader()
{
	if(ifstream_.is_open())
		ifstream_.close();
}

const LPCWSTR Loader::getFilePath()
{
	return filePath_;
}