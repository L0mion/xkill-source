#include "WriterPGY.h"

WriterPGY::WriterPGY(
	const MeshModel		subject,
	const WriteTimeUTC	writeTimeUTC,
	const std::string	filePath,
	const std::string	fileName) : Writer(filePath, fileName)
{
	writeTimeUTC_	= writeTimeUTC;
	subject_		= subject;
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
	PGYHeader header = loadHeader();
	writeHeader(header);
	
	/*Write materials*/
	writeMaterials(subject_.getMaterials());
	writeGeometry(subject_.getGeometry());
}
const PGYHeader WriterPGY::loadHeader()
{
	MeshGeometry geometry = subject_.getGeometry();
	std::vector<MeshSubset> subsets = geometry.getSubsets();

	unsigned int numMaterials	= subject_.getNumMaterials();

	unsigned int numVertices	= geometry.getNumVertices();
	unsigned int numSubsets		= geometry.getNumSubsets();
	
	PGYHeader header;
	for(unsigned int i = 0; i < 4; i++)
		header.fileType_[i]	= PGY_SPECS_FILETYPE[i];
	header.versionNum_		= WRITER_PGY_VERSION;
	header.writeTime_		= writeTimeUTC_;
	header.vertexType_		= POS_NORM_TEX;
	header.numMaterials_	= numMaterials;
	header.numVertices_		= numVertices;
	header.numSubsets_		= numSubsets;

	return header;
}
void WriterPGY::writeHeader(const PGYHeader header)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&header),
		PGY_SPECS_SIZE_HEADER);
}
void WriterPGY::writeMaterials(const std::vector<MeshMaterial> materials)
{
	for(unsigned int i = 0; i < materials.size(); i++)
		writeMaterial(materials[i]);
}
void WriterPGY::writeMaterial(const MeshMaterial material)
{
	ofstream_.write(reinterpret_cast<const char*>(&material), PGY_SPECS_SIZE_MATERIAL);
}
void WriterPGY::writeGeometry(MeshGeometry geometry)
{
	unsigned int numVertices = geometry.getNumVertices();
	std::vector<VertexPosNormTex> vertices = geometry.getVertices();
	writeVertices(
		numVertices,
		vertices);

	unsigned int numSubsets = geometry.getNumSubsets();
	std::vector<MeshSubset> subsets = geometry.getSubsets();
	writeSubsets(
		numSubsets,
		subsets);
}
void WriterPGY::writeVertices(
		const unsigned int numVertices, 
		const std::vector<VertexPosNormTex> vertices)
{
	for(unsigned int i = 0; i < numVertices; i++)
		writeVertex(vertices[i]);
}
void WriterPGY::writeVertex(const VertexPosNormTex vertex)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&vertex), 
		PGY_SPECS_SIZE_VERTEXPOSNORMTEX);
}
void WriterPGY::writeSubsets(
		const unsigned int				numSubsets, 
		const std::vector<MeshSubset>	subsets)
{
	for(unsigned int i = 0; i < numSubsets; i++)
		writeSubset(subsets[i]);
}
void WriterPGY::writeSubset(MeshSubset subset)
{
	/*Write PGY Subset header*/
	PGYHeaderSubset ssHeader;
	ssHeader.numIndices_ = subset.getNumIndices();
	ofstream_.write(
		reinterpret_cast<const char*>(&ssHeader), 
		PGY_SPECS_SIZE_HEADER_SUBSET);

	/*Write material index to PGY*/
	unsigned int materialIndex = subset.getMaterialIndex();
	ofstream_.write(
		reinterpret_cast<const char*>(&materialIndex), 
		sizeof(materialIndex));

	/*Write indices to PGY*/
	unsigned int numIndices = subset.getNumIndices();
	std::vector<unsigned int> indices = subset.getIndices();
	for(unsigned int i = 0; i < numIndices; i++)
		writeIndex(indices[i]);
}
void WriterPGY::writeIndex(unsigned int index)
{
	ofstream_.write(
		reinterpret_cast<const char*>(&index), 
		sizeof(index));
}