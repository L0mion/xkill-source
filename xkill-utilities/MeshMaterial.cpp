#include "MeshMaterial.h"

MeshMaterial::MeshMaterial()
{
	//Do nothing.
}
MeshMaterial::MeshMaterial(
	const Float3	ambientTerm,
	const Float3	diffuseTerm,
	const Float3	specularTerm,
	const float		specularPower,
	const Float3	reflectivity,
	const bool		transperency)
{
	ambientTerm_	= ambientTerm;
	diffuseTerm_	= diffuseTerm;
	specularTerm_	= specularTerm;
	specularPower_	= specularPower;
	reflectivity_	= reflectivity;
	transperency_	= transperency;
}
MeshMaterial::~MeshMaterial()
{
	//Do nothing.
}

const Float3 MeshMaterial::getAmbientTerm()
{
	return ambientTerm_;
}
const Float3 MeshMaterial::getDiffuseTerm()
{
	return diffuseTerm_;
}
const Float3 MeshMaterial::getSpecularTerm()
{
	return specularTerm_;
}
const float MeshMaterial::getSpecularPower()
{
	return specularPower_;
}
const Float3 MeshMaterial::getReflectivity()
{
	return reflectivity_;
}
const bool MeshMaterial::getTransperency()
{
	return transperency_;
}