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

//struct LightDesc
//{
//	LightDesc();
//	virtual ~LightDesc();
//
//	Float4 ambient;
//	Float4 diffuse;
//	Float4 specular;
//};

struct DLL_U LightDescDir// : public LightDesc
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

struct DLL_U LightDescPoint //: public LightDesc
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

struct DLL_U LightDescSpot //: public LightDesc
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

//! Struct describing a single light source.
/*!
\ingroup xkill-renderer
*/
//struct LightDesc
//{
//	//! Initializes LightDesc to its default state.
//	LightDesc()
//	{
//		ZeroMemory(this, sizeof(LightDesc));
//	}
//
//	Float3			position;		//!< Position of the light source.
//	float			range;			//!< How far the light can reach.
//	Float3			direction;		//!< The lights direction.
//	float			spotPower;		//!< Controls the spotlight cone.
//	Float4			ambient;		//!< The ambient color.
//	Float4			diffuse;		//!< The diffuse color.
//	Float4			specular;		//!< The specular color.
//	Float3			attenuation;	//!< How fast the light intensity will diminish
//	unsigned int	type;			//!< Type of light i.e. Directional, Point or Spot.
//};

#endif //XKILL_RENDERER_LIGHTDESC_H