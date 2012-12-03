#ifndef XKILL_RENDERER_SPECSPGY_H
#define XKILL_RENDERER_SPECSPGY_H

#include <string>

#include "MeshMaterial.h"

static const std::string	PGY_SPECS_SUFFIX			= ".pgy";
static const unsigned int	PGY_SPECS_MATERIAL_SIZE		= sizeof(MeshMaterial);

enum PGY_SPECS_VERTEX
{
	POS_NORM_TEX
};

struct PGYHeader
{
	char			fileType[4];
	float			versionNum;
	unsigned int	vertexType;		//PGY_SPECS_VERTEX
	unsigned int	materialsNum;
};

#endif //XKILL_RENDERER_SPECSPGY_H