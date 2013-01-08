#ifndef XKILL_IO_LOADERPGY_H
#define XKILL_IO_LOADERPGY_H

#include <xkill-utilities/MeshModel.h>

#include "Loader.h"
#include "SpecsPGY.h"

// Must correspond with WRITER_PGY_VERSION in order to read .pgy sucessfully.
static const float LOADER_PGY_VERSION = 1.1f;
/*
* 1.1 - Now reads WriteTimeUTC struct from PGY-header.
*/

//! MeshLoader with functionality to load .obj-files.
/*!
Loader has minimal or no error handling to ensure a fast loading of binary .pgy-format.
\ingroup xkill-mesh-io-pgy
*/
class LoaderPGY : public Loader
{
public:
	//! Forwards path to .pgy to parent Loader-class.
	/*!
	\param filePath Path up to desired .pgy-file.
	\param fileName Name of desired .pgy-file.
	*/
	LoaderPGY(
		const std::string filePath, 
		const std::string fileName);
	//! Does nothing.
	~LoaderPGY();

	//! Opens specified file and calls loading of binary .pgy-format.
	/*!
	\return Boolean dictating whether or not the method was sucessful.
	*/
	bool init();

	WriteTimeUTC	getWriteTimeUTC()	const;
	MeshModel*		getMeshModel()		const;
protected:
private:
	//! Loads binary .pgy-format.
	/*!
	\return Resulting model read from .pgy.
	\sa loadHeader
	\sa loadMaterials
	\sa loadGeometry
	*/
	MeshModel*						loadPGY();
	//! Loads static size PGYHeader from binary .pgy-format.
	/*!
	This header describes the rest of the file, and how to parse it correctly.
	\return PGYHeader read from binary .pgy-format.
	*/
	const PGYHeader						loadHeader();
	//! Loads materials from binary .pgy-format.
	/*!
	\sa loadMaterial
	\param numMaterials
	\return Vector of all materials read from binary .pgy-format.
	*/
	const std::vector<MeshMaterial>		loadMaterials(const unsigned int numMaterials);
	//! Loads a single material from binary .pgy-format.
	/*!
	\return Read material.
	*/
	const MeshMaterial					loadMaterial();
	//! Reads geometry-data from binary .pgy-format.
	/*!
	\sa loadVertices
	\sa loadSubsets
	\param numVertices Number of vertices expected.
	\param numSubsets Number of subsets expected.
	\return Read geometry.
	*/
	const MeshGeometry					loadGeometry(
		const unsigned int			numVertices,
		const unsigned int			numSubsets);
	//! Reads vertices from .pgy.
	/*!
	\sa loadVertex
	\param numVertices Number of vertices expected.
	\return Read vertices.
	*/
	const std::vector<VertexPosNormTex>	loadVertices(const unsigned int numVertices);
	//! Reads a single vertex from .pgy.
	/*! \return Read vertex. */
	const VertexPosNormTex				loadVertex();
	//! Subsets from .pgy.
	/*!
	\sa loadSubset
	\param numSubsets Number of vertices expected.
	\return Read subsets.
	*/
	const std::vector<MeshSubset>		loadSubsets(const unsigned int numSubsets);
	//! Reads a single subset from .pgy.
	/*! \return Read subset. */
	const MeshSubset					loadSubset();

	WriteTimeUTC writeTimeUTC_;

	MeshModel* meshModel_; //!< Resulting model read from .pgy.
};

#endif //XKILL_IO_LOADERPGY_H