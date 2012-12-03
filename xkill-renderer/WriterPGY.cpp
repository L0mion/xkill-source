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
		writePGY();
		ofstream_.close();
	}

	return true;
}

void WriterPGY::writePGY()
{
	/*Write PGY header*/
	unsigned int numMaterials = subject_.getNumMaterials();
	writeHeader(
		POS_NORM_TEX,
		WRITER_PGY_VERSION,
		numMaterials);

	/*Write materials*/
	writeMaterials(subject_.getMaterials());
}
void WriterPGY::writeHeader(
	const PGY_SPECS_VERTEX	vertexType,
	const unsigned int		versionNum,
	const unsigned int		numMaterials)
{
	PGYHeader header;
	for(unsigned int i = 0; i < 4; i++)
		header.fileType[i]	= PGY_SPECS_FILETYPE[i];
	header.versionNum		= WRITER_PGY_VERSION;
	header.vertexType		= vertexType;
	header.materialsNum		= numMaterials;

	ofstream_.write(reinterpret_cast<const char*>(&header), PGY_SPECS_HEADER_SIZE);
}
void WriterPGY::writeMaterials(const std::vector<MeshMaterial> materials)
{
	for(unsigned int i = 0; i < materials.size(); i++)
	{
		MeshMaterial material = materials[i];
		ofstream_.write(reinterpret_cast<const char*>(&material), PGY_SPECS_MATERIAL_SIZE);
	}
}