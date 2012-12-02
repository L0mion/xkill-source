#include "PGYFormat.h"

#include "WriterPGY.h"

WriterPGY::WriterPGY(
	const MeshModel		subject,
	const std::string	filePath,
	const std::string	fileName) : Writer(filePath, fileName)
{
	subject_ = subject;
}
WriterPGY::~WriterPGY()
{
	//Do nothing.
}

bool WriterPGY::init()
{
	bool sucessfulWrite = true;

	ofstream_ = std::ofstream(
		getFullPath(), 
		std::ios::out | std::ios::binary);

	if(!ofstream_.is_open())
		sucessfulWrite = false;
	else
	{
		writeHeader();

		ofstream_.close();
	}

	return true;
}

void WriterPGY::writeHeader()
{
	char fileType[4] = "pgy";
	PGYHeader header;
	for(unsigned int i = 0; i < 4; i++)
		header.fileType[i] = fileType[i];
	header.versionNum = 0.1f;

	ofstream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
}