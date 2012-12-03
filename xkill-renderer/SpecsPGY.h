#ifndef XKILL_RENDERER_SPECSPGY_H
#define XKILL_RENDERER_SPECSPGY_H

#include <string>

#include "MeshMaterial.h"

static const std::string	PGY_SPECS_SUFFIX			= ".pgy";
static const unsigned int	PGY_SPECS_MATERIAL_SIZE		= sizeof(MeshMaterial);

struct PGYHeader
{
	char	fileType[4];
	float	versionNum;
};

#endif //XKILL_RENDERER_SPECSPGY_H