#ifndef XKILL_RENDERER_MESHMATERIAL_H
#define XKILL_RENDERER_MESHMATERIAL_H

#include <DirectXMath.h>

//! Describes material to be applied onto a subset of mesh.
/*!
Corresponds to a single MeshSubset.
\sa MeshSubset
\ingroup xkill-mesh
*/
class MeshMaterial
{
public:
	//! Does nothing.
	MeshMaterial();
	//! Initializes Material to data parsed as arguments.
	MeshMaterial(
		const DirectX::XMFLOAT3	mmAmbientTerm,
		const DirectX::XMFLOAT3	mmDiffuseTerm,
		const DirectX::XMFLOAT3	mmSpecularTerm,
		const float				mmSpecularPower,
		const DirectX::XMFLOAT3	mmReflectivity,
		const bool				mmTransperency);
	//! Does nothing.
	~MeshMaterial();

	const DirectX::XMFLOAT3	getMMAmbientTerm();
	const DirectX::XMFLOAT3	getMMDiffuseTerm();
	const DirectX::XMFLOAT3	getMMSpecularTerm();
	const float				getMMSpecularPower();
	const DirectX::XMFLOAT3	getMMReflectivity();
	const bool				getMMTransperency();
protected:
private:
	DirectX::XMFLOAT3	mmAmbientTerm_;		//!< Ambient term to be applied unto subset.
	DirectX::XMFLOAT3	mmDiffuseTerm_;		//!< Diffuse term to be applied unto subset.
	DirectX::XMFLOAT3	mmSpecularTerm_;	//!< Specular term to be applied unto subset.
	float				mmSpecularPower_;	//!< Specular power to affect lighting applied unto subset.
	DirectX::XMFLOAT3	mmReflectivity_;	//!< Reflectivity to be applied unto subset.
	bool				mmTransperency_;	//!< Whether subset is transparent or not.
};

#endif //XKILL_RENDERER_MESHMATERIAL_H