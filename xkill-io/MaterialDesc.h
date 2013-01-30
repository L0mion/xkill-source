#ifndef XKILL_IO_MATERIALDESC_H
#define XKILL_IO_MATERIALDESC_H

#include <xkill-utilities/Math.h>

struct MaterialDesc
{
	MaterialDesc();
	MaterialDesc(
		const Float3	ambientTerm,
		const Float3	diffuseTerm,
		const Float3	specularTerm,
		const float		specularPower,
		const Float3	reflectivity,
		const bool		transperency,

		const unsigned int idAlbedoTex,
		const unsigned int idNormalTex);
	//! Does nothing.
	~MaterialDesc();

	Float3	ambientTerm_;		//!< Ambient term to be applied unto subset.
	Float3	diffuseTerm_;		//!< Diffuse term to be applied unto subset.
	Float3	specularTerm_;		//!< Specular term to be applied unto subset.
	float	specularPower_;		//!< Specular power to affect lighting applied unto subset.
	Float3	reflectivity_;		//!< Reflectivity to be applied unto subset.
	bool	transperency_;		//!< Whether subset is transparent or not.

	Float3 emissive_;
	float shinyness_;

	/*Textures*/
	unsigned int idAlbedoTex_; //!< Albedo texture.
	unsigned int idNormalTex_; //!< Normal texture.
};

#endif //XKILL_IO_MATERIALDESC_H