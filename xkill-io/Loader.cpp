#include <string>
#include <sstream>

#include "Loader.h"

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

bool Loader::isNumeric(const std::string value)
{
	std::stringstream conv;
	double tmp;
	conv << value;
	conv >> tmp;
	return conv.eof();
}
void Loader::getLine(std::string& line)
{
	std::getline(ifstream_, line);
	lineNum_++;
}

const std::string Loader::getFilePath()
{
	return filePath_;
}
const std::string Loader::getFileName()
{
	return fileName_;
}
const std::string Loader::getFullPath()
{
	return filePath_ + fileName_;
}