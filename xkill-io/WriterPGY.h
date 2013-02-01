#ifndef XKILL_IO_WRITERPGY_H
#define XKILL_IO_WRITERPGY_H

#include <string>
#include <xkill-utilities/MeshDesc.h>

#include "Writer.h"
#include "SpecsPGY.h"
#include "WriteTime.h"

// !OBS! Be sure to increment WRITER_PGY_VERSION if updating WriterPGY! !OBS!
static const float WRITER_PGY_VERSION = 1.2f;
/*
* 1.1 - Now writes WriteTimeUTC struct to PGY-header.
*/

//! Writer with functionality to write binary .pgy-files.
/*! !OBS! Be sure to increment WRITER_PGY_VERSION if updating WriterPGY! !OBS!
\ingroup xkill-mesh-io-pgy
*/
class WriterPGY : public Writer
{
public:
	WriterPGY(
		const MeshDesc		subject,
		//const WriteTimeUTC	writeTimeUTC,
		const std::string	filePath,
		const std::string	fileName,
		const VertexType	vertexType); //!< Forwards path to .obj to parent Loader-class.
	~WriterPGY(); //!< Does nothing.

	bool init(); //! Function initializing writer.
protected:
private:
	void writePGY(); //!< Writes passed model to binary format.
	const PGYHeader loadHeader(
		unsigned int numMaterials, 
		unsigned int numVertices, 
		unsigned int numSubsets); //!< Returns correct header-data which will then be written to file.
	
	void writeHeader(const PGYHeader header); //!< Writes header to file. Always done first when writing .pgy.
	void writeMaterials(const std::vector<MaterialDesc> materials); //!< Writes materials to binary format.
	void writeMaterial(const MaterialDesc material); //!< Writes a single material to binary .pgy.
	
	void writeGeometry(
		std::vector<VertexDesc> vertices, 
		std::vector<SubsetDesc> subsets); //!< Writes geometry of mesh to .pgy.
	void writeVertices(
		const unsigned int numVertices, 
		const std::vector<VertexDesc>	vertices); //!< Writes vertices to .pgy.
	void writeVertex(const VertexDesc vertex); //!< Writes a single vertex to .pgy.
	void writeSubsets(
		const unsigned int				numSubsets, 
		const std::vector<SubsetDesc>	subsets); //!< Writes subsets to .pgy.
	
	void writeSubset(SubsetDesc subset); //!< Writes a single subset to .pgy.
	void writeIndex(unsigned int index); //!< Writes a single index to .pgy.

	MeshDesc		subject_;		//!< Model to be written to .pgy.
	WriteTimeUTC	writeTimeUTC_;	//!< Time last written to original file being converted into .pgy. Measured in UTC.
	VertexType		subjectVertexType_;
};

#endif //XKILL_IO_WRITERPGY_H