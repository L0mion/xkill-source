#ifndef XKILL_RENDERER_WRITERPGY_H
#define XKILL_RENDERER_WRITERPGY_H

#include <string>
#include <xkill-utilities/MeshModel.h>

#include "Writer.h"
#include "SpecsPGY.h"

static const float WRITER_PGY_VERSION = 0.1f;

//! Writer with functionality to write binary .pgy-files.
/*!
\ingroup xkill-mesh-io-pgy
*/
class WriterPGY : public Writer
{
public:
	//! Forwards path to .obj to parent Loader-class.
	/*!
	\param subject	Model to condense into binary .pgy-format.
	\param filePath Path up to location to place .pgy-file.
	\param fileName Name of desired .pgy-file.
	*/
	WriterPGY(
		const MeshModel		subject,
		const std::string	filePath,
		const std::string	fileName);
	//! Does nothing.
	~WriterPGY();

	//! Function initializing writer.
	/*!
	Method attempts to create specified file and then proceed to write to binary format.
	\sa writePGY
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool init();
protected:
private:
	//! Writes passed model to binary format.
	/*!
	\sa loadHeader
	\sa writeHeader
	\sa writeMaterials
	\sa writeGeometry
	*/
	void writePGY();
	//! Returns correct header-data which will then be written to file.
	const PGYHeader loadHeader();
	//! Writes header to file. Always done first when writing .pgy.
	void writeHeader(const PGYHeader header);
	//! Writes materials to binary format.
	/*!	\sa writeMaterial	*/
	void writeMaterials(const std::vector<MeshMaterial> materials);
	//! Writes a single material to binary .pgy.
	void writeMaterial(const MeshMaterial material);
	//! Writes geometry of mesh to .pgy.
	/*!
	\sa writeVertices
	\sa writeSubsets
	*/
	void writeGeometry(MeshGeometry geometry);
	//! Writes vertices to .pgy.
	/*!	\sa writeVertex	*/
	void writeVertices(
		const unsigned int					numVertices, 
		const std::vector<VertexPosNormTex>	vertices);
	//! Writes a single vertex to .pgy.
	void writeVertex(const VertexPosNormTex vertex);
	//! Writes subsets to .pgy.
	/*!	\sa writeSubset	*/
	void writeSubsets(
		const unsigned int				numSubsets, 
		const std::vector<MeshSubset>	subsets);
	//! Writes a single subset to .pgy.
	/*! Writes a PGYHeaderSubset struct to .pgy beforehand to dictate number of indices in each subset. \sa writeIndex */
	void writeSubset(MeshSubset subset);
	//! Writes a single index to .pgy.
	void writeIndex(unsigned int index);

	MeshModel subject_; //!< Model to be written to .pgy.
};

#endif //XKILL_RENDERER_WRITERPGY_H