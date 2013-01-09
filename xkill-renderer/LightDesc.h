#ifndef XKILL_RENDERER_LIGHTDESC_H
#define XKILL_RENDERER_LIGHTDESC_H

#include <DirectXMath.h>


static const unsigned int LIGHT_DIRECTIONAL = 1;
static const unsigned int LIGHT_POINT		= 2;
static const unsigned int LIGHT_SPOT		= 3;

//! Struct describing a single light source.
/*!
\ingroup xkill-renderer
*/
struct LightDesc
{
	//! Initializes LightDesc to its default state.
	LightDesc()
	{
		ZeroMemory(this, sizeof(LightDesc));
	}

	Float3			position;		//!< Position of the light source.
	float			range;			//!< How far the light can reach.
	Float3			direction;		//!< The lights direction.
	float			spotPower;		//!< Controls the spotlight cone.
	Float4			ambient;		//!< The ambient color.
	Float4			diffuse;		//!< The diffuse color.
	Float4			specular;		//!< The specular color.
	Float3			attenuation;	//!< How fast the light intensity will diminish
	unsigned int	type;			//!< Type of light i.e. Directional, Point or Spot.

	/*
	DirectX::XMFLOAT3 position;		//!< Position of the light source.
	float range;					//!< How far the light can reach.
	DirectX::XMFLOAT3 direction;	//!< The lights direction.
	float spotPower;				//!< Controls the spotlight cone.
	DirectX::XMFLOAT4 ambient;		//!< The ambient color.
	DirectX::XMFLOAT4 diffuse;		//!< The diffuse color.
	DirectX::XMFLOAT4 specular;		//!< The specular color.
	DirectX::XMFLOAT3 attenuation;	//!< How fast the light intensity will diminish
	unsigned int type;				//!< Type of light i.e. Directional, Point or Spot.
	*/
};

#endif //XKILL_RENDERER_LIGHTDESC_H