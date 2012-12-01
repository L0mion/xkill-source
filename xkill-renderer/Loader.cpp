#include <string>

#include <Loader.h>

Loader::Loader(
	const std::string filePath,
	const std::string fileName)
{
	filePath_ = filePath;
	fileName_ = fileName;
}
Loader::~Loader()
{
	if(ifstream_.is_open())
		ifstream_.close();
}

const std::string Loader::getFilePath()
{
	return filePath_;
}
const std::string Loader::getFileName()
{
	return fileName_;
}