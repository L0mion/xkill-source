#include "Writer.h"

Writer::Writer(
	const std::string filePath,
	const std::string fileName)
{
	filePath_ = filePath;
	fileName_ = fileName;
}
Writer::~Writer()
{
	if(ofstream_.is_open())
		ofstream_.close();
}

const std::string Writer::getFilePath()
{
	return filePath_;
}
const std::string Writer::getFileName()
{
	return fileName_;
}
const std::string Writer::getFullPath()
{
	return filePath_ + fileName_;
}