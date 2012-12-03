#ifndef XKILL_RENDERER_SPECSPGY_H
#define XKILL_RENDERER_SPECSPGY_H

#include <string>

#include "MeshMaterial.h"
#include "MeshSubset.h"

enum PGY_SPECS_VERTEX
{
	POS_NORM_TEX
};

struct PGYHeader
{
	char						fileType_[4];
	float						versionNum_;
	unsigned int				vertexType_;		//PGY_SPECS_VERTEX
	unsigned int				numMaterials_;
	unsigned int				numVertices_;
	unsigned int				numSubsets_;
};
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