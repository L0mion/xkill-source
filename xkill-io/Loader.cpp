#include <string>
#include <sstream>

#include "Loader.h"

Loader::Loader(
	const std::string filePath,
	const std::string fileName)
{
	filePath_ = filePath;
	fileName_ = fileName;

	lastError_ = LOADER_ERROR_NA;
}
Loader::~Loader()
{
	if(ifstream_.is_open())
		ifstream_.close();
}

LOADER_ERROR Loader::getLastError() const
{
	return lastError_;
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

std::string Loader::getFilePath() const
{
	return filePath_;
}
std::string Loader::getFileName() const
{
	return fileName_;
}
std::string Loader::getFullPath() const
{
	return filePath_ + fileName_;
}