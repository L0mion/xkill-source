#include <xkill-utilities/Util.h>

#include "WriterPGY.h"

WriterPGY::WriterPGY(
	const MeshDesc		subject,
	//const WriteTimeUTC	writeTimeUTC,
	const std::string	filePath,
	const std::string	fileName,
	const VertexType	vertexType) : Writer(filePath, fileName)
{
	//writeTimeUTC_	= writeTimeUTC;
	subject_		= subject;
	subjectVertexType_ = vertexType;
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

	return sucessfulWrite;
}

void WriterPGY::writePGY()
{
	unsigned int numMaterials = subject_.materials_.size();
	unsigned int numVertices = subject_.vertices_.size();
	unsigned int numSubsets	= subject_.subsets_.size();

	/*Write PGY header*/
	PGYHeader header = loadHeader(numMaterials, numVertices, numSubsets);
	writeHeader(header);
	
	/*Write materials*/
	writeMaterials(subject_.materials_);
	writeGeometry(subject_.vertices_, subject_.subsets_);
}
const PGYHeader WriterPGY::loadHeader(
	unsigned int numMaterials,
	unsigned int numVertices, 
	unsigned int numSubsets)
{
	PGYHeader header;
	for(unsigned int i = 0; i < 4; i++)
		header.fileType_[i]	= PGY_SPECS_FILETYPE[i];
	header.versionNum_		= WRITER_PGY_VERSION;
	header.writeTime_		= writeTimeUTC_;
	header.vertexType_		= subjectVertexType_;
	header.numMaterials_	= numMaterials;
	header.numVertices_		= numVertices;
	header.numSubsets_		= numSubsets;

	return header;
}
void WriterPGY::writeHeader(const PGYHeader header)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&header),
		sizeof(PGYHeader));

	std::string log = "Wrote a " + sizeof(PGYHeader);
	log += " bytes PGY-header to PGY-file with version number " + std::to_string(header.versionNum_) + ". \n";
	DEBUGPRINT(log);
}
void WriterPGY::writeMaterials(const std::vector<MaterialDesc> materials)
{
	for(unsigned int i = 0; i < materials.size(); i++)
		writeMaterial(materials[i]);

	std::string log = "Wrote " + materials.size();
	log += " materials to PGY-file. \n";
	DEBUGPRINT(log);
}
void WriterPGY::writeMaterial(const MaterialDesc material)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&material), 
		sizeof(MaterialDesc));
}
void WriterPGY::writeGeometry(std::vector<VertexDesc> vertices, 
							  std::vector<SubsetDesc> subsets)
{
	unsigned int numVertices = vertices.size();
	writeVertices(
		numVertices,
		vertices);

	unsigned int numSubsets = subsets.size();
	writeSubsets(
		numSubsets,
		subsets);
}
void WriterPGY::writeVertices(
	const unsigned int numVertices, 
	const std::vector<VertexDesc>	vertices)
{
	for(unsigned int i = 0; i < numVertices; i++)
		writeVertex(vertices[i]);

	std::string log = "Wrote " + numVertices;
	log += " vertices to PGY-file.";
	DEBUGPRINT(log);
}
void WriterPGY::writeVertex(const VertexDesc vertex)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&vertex), 
		sizeof(VertexDesc));
}
void WriterPGY::writeSubsets(
		const unsigned int				numSubsets, 
		const std::vector<SubsetDesc>	subsets)
{
	for(unsigned int i = 0; i < numSubsets; i++)
		writeSubset(subsets[i]);

	std::string log = "Wrote " + numSubsets;
	log += " subsets to PGY-file. \n";
	DEBUGPRINT(log);
}
void WriterPGY::writeSubset(SubsetDesc subset)
{
	/*Write PGY Subset header*/
	PGYHeaderSubset ssHeader;
	ssHeader.numIndices_ = subset.indices_.size();
	ofstream_.write(
		reinterpret_cast<const char*>(&ssHeader), 
		sizeof(PGYHeaderSubset));

	/*Write material index to PGY*/
	unsigned int materialIndex = subset.materialIndex_;
	ofstream_.write(
		reinterpret_cast<const char*>(&materialIndex), 
		sizeof(materialIndex));

	/*Write indices to PGY*/
	unsigned int numIndices = subset.indices_.size();
	std::vector<unsigned int> indices = subset.indices_;
	for(unsigned int i = 0; i < numIndices; i++)
		writeIndex(indices[i]);
}
void WriterPGY::writeIndex(unsigned int index)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&index), 
		sizeof(index));
}