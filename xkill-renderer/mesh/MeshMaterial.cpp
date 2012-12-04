#include <MeshMaterial.h>

MeshMaterial::MeshMaterial()
{
	//Do nothing.
}
MeshMaterial::MeshMaterial(
	const DirectX::XMFLOAT3	mmAmbientTerm,
	const DirectX::XMFLOAT3	mmDiffuseTerm,
	const DirectX::XMFLOAT3	mmSpecularTerm,
	const float				mmSpecularPower,
	const DirectX::XMFLOAT3	mmReflectivity,
	const bool				mmTransperency)
{
	mmAmbientTerm_		= mmAmbientTerm;
	mmDiffuseTerm_		= mmDiffuseTerm;
	mmSpecularTerm_		= mmSpecularTerm;
	mmSpecularPower_	= mmSpecularPower;
	mmReflectivity_		= mmReflectivity;
	mmTransperency_		= mmTransperency;
}
MeshMaterial::~MeshMaterial()
{
	//Do nothing.
}

const DirectX::XMFLOAT3 MeshMaterial::getMMAmbientTerm()
{
	return mmAmbientTerm_;
}
const DirectX::XMFLOAT3 MeshMaterial::getMMDiffuseTerm()
{
	return mmDiffuseTerm_;
}
const DirectX::XMFLOAT3 MeshMaterial::getMMSpecularTerm()
{
	return mmSpecularTerm_;
}
const float MeshMaterial::getMMSpecularPower()
{
	return mmSpecularPower_;
}
const DirectX::XMFLOAT3 MeshMaterial::getMMReflectivity()
{
	return mmReflectivity_;
}
const bool MeshMaterial::getMMTransperency()
{
	return mmTransperency_;
}