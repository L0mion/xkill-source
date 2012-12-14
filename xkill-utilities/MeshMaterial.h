#ifndef XKILL_RENDERER_MESHMATERIAL_H
#define XKILL_RENDERER_MESHMATERIAL_H

#include "dllUtilities.h"
#include "Math.h"

//! Describes material to be applied onto a subset of mesh.
/*!
Corresponds to a single MeshSubset.
\sa MeshSubset
\ingroup xkill-mesh
*/
class DLL_U MeshMaterial
{
public:
	//! Initializes default material.
	MeshMaterial();
	//! Initializes Material to data parsed as arguments.
	MeshMaterial(
		const Float3	ambientTerm,
		const Float3	diffuseTerm,
		const Float3	specularTerm,
		const float		specularPower,
		const Float3	reflectivity,
		const bool		transperency,
		
		const unsigned int idAlbedoTex,
		const unsigned int idNormalTex);
	//! Does nothing.
	~MeshMaterial();

	Float3	getAmbientTerm()	const;
	Float3	getDiffuseTerm()	const;
	Float3	getSpecularTerm()	const;
	float	getSpecularPower()	const;
	Float3	getReflectivity()	const;
	bool	getTransperency()	const;

	unsigned int getIDAlbedoTex() const;
	unsigned int getIDNormalTex() const;
protected:
private:
	Float3	ambientTerm_;		//!< Ambient term to be applied unto subset.
	Float3	diffuseTerm_;		//!< Diffuse term to be applied unto subset.
	Float3	specularTerm_;		//!< Specular term to be applied unto subset.
	float	specularPower_;		//!< Specular power to affect lighting applied unto subset.
	Float3	reflectivity_;		//!< Reflectivity to be applied unto subset.
	bool	transperency_;		//!< Whether subset is transparent or not.

	/*Textures*/
	unsigned int idAlbedoTex_; //!< Albedo texture.
	unsigned int idNormalTex_; //!< Normal texture.
};

#endif //XKILL_RENDERER_MESHMATERIAL_H