#include <string>

#include <Loader.h>

Loader::Loader(
	const LPCWSTR filePath,
	const LPCWSTR fileName)
{
	filePath_ = filePath;
	fileName_ = fileName;
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
const LPCWSTR Loader::getFileName()
{
	return fileName_;
}