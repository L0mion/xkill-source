#ifndef XKILL_RENDERER_LIGHTDESC_H
#define XKILL_RENDERER_LIGHTDESC_H

#include "dllUtilities.h"
#include "Math.h"

static const unsigned int LIGHT_DIRECTIONAL = 1;
static const unsigned int LIGHT_POINT		= 2;
static const unsigned int LIGHT_SPOT		= 3;

enum DLL_U LightDescType
{ 
	LIGHTDESCTYPE_DIR	= 0, 
	LIGHTDESCTYPE_POINT	= 1, 
	LIGHTDESCTYPE_SPOT	= 2, 
	LIGHTDESCTYPE_NA	= 3
};

struct DLL_U LightDescDir
{
	LightDescDir();
	~LightDescDir();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	Float3 direction;

	//Padding
	float pad1;
};

struct DLL_U LightDescPoint
{
	LightDescPoint();
	~LightDescPoint();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	Float3	pos;
	float	range;
	Float3	attenuation;

	//Padding
	float pad1;
};

struct DLL_U LightDescSpot
{
	LightDescSpot();
	~LightDescSpot();

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	Float3	pos;
	float	range;
	Float3	direction;
	float	spotPow;
	Float3	attenuation;

	//Padding
	float pad1;
};

#endif //XKILL_RENDERER_LIGHTDESC_H