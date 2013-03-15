#ifndef XKILL_RENDERER_LIGHTDESC_H
#define XKILL_RENDERER_LIGHTDESC_H

#include "dllUtilities.h"
#include "Math.h"

static const unsigned int LIGHT_DIRECTIONAL = 1;
static const unsigned int LIGHT_POINT		= 2;

struct DLL_U LightDescDir
{
	LightDescDir();
	~LightDescDir();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;
};

struct DLL_U LightDescPoint
{
	LightDescPoint();
	~LightDescPoint();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	Float3	attenuation;
	float	range;
};

#endif //XKILL_RENDERER_LIGHTDESC_H