#include "MeshMaterial.h"

MeshMaterial::MeshMaterial()
{
	ambientTerm_	= Float3(1.0f, 1.0f, 1.0f);
	diffuseTerm_	= Float3(1.0f, 1.0f, 1.0f);
	specularTerm_	= Float3(1.0f, 1.0f, 1.0f);
	specularPower_	= 1.0f;
	reflectivity_	= Float3(0.5f, 0.5f, 0.5f);
	transperency_	= false;

	idAlbedoTex_ = 0;
	idNormalTex_ = 0;
}
MeshMaterial::MeshMaterial(
	const Float3	ambientTerm,
	const Float3	diffuseTerm,
	const Float3	specularTerm,
	const float		specularPower,
	const Float3	reflectivity,
	const bool		transperency,
	
	const unsigned int idAlbedoTex,
	const unsigned int idNormalTex)
{
	ambientTerm_	= ambientTerm;
	diffuseTerm_	= diffuseTerm;
	specularTerm_	= specularTerm;
	specularPower_	= specularPower;
	reflectivity_	= reflectivity;
	transperency_	= transperency;

	idAlbedoTex_ = idAlbedoTex;
	idNormalTex_ = idNormalTex;
}
MeshMaterial::~MeshMaterial()
{
	//Do nothing.
}

Float3 MeshMaterial::getAmbientTerm()	const
{
	return ambientTerm_;
}
Float3 MeshMaterial::getDiffuseTerm()	const
{
	return diffuseTerm_;
}
Float3 MeshMaterial::getSpecularTerm()	const
{
	return specularTerm_;
}
float MeshMaterial::getSpecularPower()	const
{
	return specularPower_;
}
Float3 MeshMaterial::getReflectivity()	const
{
	return reflectivity_;
}
bool MeshMaterial::getTransperency()	const
{
	return transperency_;
}

unsigned int MeshMaterial::getIDAlbedoTex() const
{
	return idAlbedoTex_;
}
unsigned int MeshMaterial::getIDNormalTex() const
{
	return idNormalTex_;
}