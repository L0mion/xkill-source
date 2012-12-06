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
	//! Does nothing.
	MeshMaterial();
	//! Initializes Material to data parsed as arguments.
	MeshMaterial(
		const Float3	ambientTerm,
		const Float3	diffuseTerm,
		const Float3	specularTerm,
		const float		specularPower,
		const Float3	reflectivity,
		const bool		transperency);
	//! Does nothing.
	~MeshMaterial();

	const Float3	getAmbientTerm();
	const Float3	getDiffuseTerm();
	const Float3	getSpecularTerm();
	const float		getSpecularPower();
	const Float3	getReflectivity();
	const bool		getTransperency();
protected:
private:
	Float3	ambientTerm_;		//!< Ambient term to be applied unto subset.
	Float3	diffuseTerm_;		//!< Diffuse term to be applied unto subset.
	Float3	specularTerm_;	//!< Specular term to be applied unto subset.
	float	specularPower_;	//!< Specular power to affect lighting applied unto subset.
	Float3	reflectivity_;	//!< Reflectivity to be applied unto subset.
	bool	transperency_;	//!< Whether subset is transparent or not.
};

#endif //XKILL_RENDERER_MESHMATERIAL_H