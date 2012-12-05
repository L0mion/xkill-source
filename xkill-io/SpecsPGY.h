#ifndef XKILL_RENDERER_SPECSPGY_H
#define XKILL_RENDERER_SPECSPGY_H

/*! \defgroup xkill-mesh-io-pgy xkill-mesh-io-pgy
	Loading and writing of binary .pgy-files.
	\ingroup xkill-mesh-io
*/

#include <string>

#include <xkill-utilities/MeshMaterial.h>
#include <xkill-utilities/MeshSubset.h>

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

static const std::string	PGY_SPECS_SUFFIX				= ".pgy";
static const char			PGY_SPECS_FILETYPE[4]			= "pgy";

static const unsigned int	PGY_SPECS_SIZE_HEADER			= sizeof(PGYHeader);
static const unsigned int	PGY_SPECS_SIZE_HEADER_SUBSET	= sizeof(PGYHeaderSubset);
static const unsigned int	PGY_SPECS_SIZE_MATERIAL			= sizeof(MeshMaterial);
static const unsigned int	PGY_SPECS_SIZE_VERTEXPOSNORMTEX	= sizeof(VertexPosNormTex);

#endif //XKILL_RENDERER_SPECSPGY_H