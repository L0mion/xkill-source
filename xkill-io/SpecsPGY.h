#ifndef XKILL_IO_SPECSPGY_H
#define XKILL_IO_SPECSPGY_H

/*! \defgroup xkill-mesh-io-pgy xkill-mesh-io-pgy
	Loading and writing of binary .pgy-files.
	\ingroup xkill-mesh-io
*/

#include <string>

#include "WriteTime.h"

/** Types of vertices that one may read from .pgy.
* Currently supports only one vertex-type.
*/
enum PGY_SPECS_VERTEX
{
	POS_NORM_TEX
};

//! Specifies header of binary .pgy. Will always be first when reading from .pgy.
/*!
\ingroup xkill-mesh-io-pgy
*/
struct PGYHeader
{
	char			fileType_[4];
	float			versionNum_;
	WriteTimeUTC	writeTime_;
	unsigned int	vertexType_;		//PGY_SPECS_VERTEX
	unsigned int	numMaterials_;
	unsigned int	numVertices_;
	unsigned int	numSubsets_;
};

//! Specifies header of subset. This header may be found before each consecutive subset.
/*!
\ingroup xkill-mesh-io-pgy
*/
struct PGYHeaderSubset
{
	unsigned int numIndices_;
};

struct PGYHeaderSkinnedData
{
	unsigned int numAnimations_;
	unsigned int numBones_;
};

struct PGYHeaderAnimation
{
	unsigned int nameSize_;
};

struct PGYHeaderBone
{
	unsigned int numKeyframes_;
};


static const std::string	PGY_SPECS_SUFFIX				= ".pgy";
static const char			PGY_SPECS_FILETYPE[4]			= "pgy";

#endif //XKILL_IO_SPECSPGY_H