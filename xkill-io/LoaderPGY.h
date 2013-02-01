#ifndef XKILL_IO_LOADERPGY_H
#define XKILL_IO_LOADERPGY_H

#include <xkill-utilities/MeshDesc.h>

#include "Loader.h"
#include "SpecsPGY.h"
#include "VarStatus.h"

// Must correspond with WRITER_PGY_VERSION in order to read .pgy sucessfully.
static const float LOADER_PGY_VERSION = 1.2f;
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
	~LoaderPGY(); //!< Clears memory allocated by LoaderPGY.

	bool init(); //!< Opens specified file and calls loading of binary .pgy-format if the header version number corresponds with the version num of the LoaderPGY.

	MeshDesc getMeshModel();

	WriteTimeUTC	getWriteTimeUTC()	const;
protected:
private:
	MeshDesc loadPGY(
		unsigned int numMaterials,
		unsigned int numVertices,
		unsigned int numSubsets); //!< Loads binary .pgy-format.
	const PGYHeader	loadHeader(); //!< Loads static size PGYHeader from binary .pgy-format.
	const std::vector<MaterialDesc> loadMaterials(const unsigned int numMaterials); //!< Loads materials from binary .pgy-format.
	MaterialDesc loadMaterial(); //!< Loads a single material from binary .pgy-format.
	void loadGeometry(
		const unsigned int numVertices,
		const unsigned int numSubsets,
		std::vector<VertexDesc>& vertices,
		std::vector<SubsetDesc>& subsets); //!< Reads geometry-data from binary .pgy-format.
	void loadVertices(const unsigned int numVertices, std::vector<VertexDesc>& vertices); //!< Reads vertices from .pgy.
	const VertexDesc loadVertex(); //!< Reads a single vertex from .pgy.
	void loadSubsets(const unsigned int numSubsets, std::vector<SubsetDesc>& subsets); //!< Subsets from .pgy.
	const SubsetDesc loadSubset(); //!< Reads a single subset from .pgy.

	WriteTimeUTC writeTimeUTC_;

	MeshDesc meshDesc_; //!< Resulting model read from .pgy.
};

#endif //XKILL_IO_LOADERPGY_H