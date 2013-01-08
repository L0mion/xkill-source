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
/*! !OBS! Be sure to increment LOADER_PGY_VERSION if updating LoaderPGY! !OBS!
Loader has minimal error handling to ensure a fast loading of binary .pgy-format.
\ingroup xkill-mesh-io-pgy
*/
class LoaderPGY : public Loader
{
public:
	LoaderPGY(
		const std::string filePath, 
		const std::string fileName); //!< Forwards path to .pgy to parent Loader-class.
	~LoaderPGY(); //!< Does nothing.

	bool init(); //!< Opens specified file and calls loading of binary .pgy-format if the header version number corresponds with the version num of the LoaderPGY.

	WriteTimeUTC	getWriteTimeUTC()	const;
	MeshModel*		getMeshModel()		const;
protected:
private:
	MeshModel* loadPGY(
		unsigned int numMaterials,
		unsigned int numVertices,
		unsigned int numSubsets); //!< Loads binary .pgy-format.
	const PGYHeader	loadHeader(); //!< Loads static size PGYHeader from binary .pgy-format.
	const std::vector<MeshMaterial> loadMaterials(const unsigned int numMaterials); //!< Loads materials from binary .pgy-format.
	const MeshMaterial loadMaterial(); //!< Loads a single material from binary .pgy-format.
	const MeshGeometry loadGeometry(
		const unsigned int numVertices,
		const unsigned int numSubsets); //!< Reads geometry-data from binary .pgy-format.
	const std::vector<VertexPosNormTex>	loadVertices(const unsigned int numVertices); //!< Reads vertices from .pgy.
	const VertexPosNormTex loadVertex(); //!< Reads a single vertex from .pgy.
	const std::vector<MeshSubset> loadSubsets(const unsigned int numSubsets); //!< Subsets from .pgy.
	const MeshSubset loadSubset(); //!< Reads a single subset from .pgy.

	WriteTimeUTC writeTimeUTC_;

	MeshModel* meshModel_; //!< Resulting model read from .pgy.
};

#endif //XKILL_IO_LOADERPGY_H