#ifndef XKILL_RENDERER_LIGHTDESC_H
#define XKILL_RENDERER_LIGHTDESC_H

#include "dllUtilities.h"
#include "Math.h"

static const unsigned int LIGHT_DIRECTIONAL = 1;
static const unsigned int LIGHT_POINT		= 2;
static const unsigned int LIGHT_SPOT		= 3;

struct DLL_U LightDescDir
{
	LightDescDir();
	~LightDescDir();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	Float3 direction;

	//Padding
	float pad1; //remove me?
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

struct DLL_U LightDescSpot
{
	LightDescSpot();
	~LightDescSpot();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;
	
	Float3	direction;
	float	range;
	Float3	attenuation;
	float	spotPow;
};

#endif //XKILL_RENDERER_LIGHTDESC_H