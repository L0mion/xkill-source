#include <MeshDependencies.h>

MeshDependencies::MeshDependencies(
	const LPCWSTR mdTexAlbedo,
	const LPCWSTR mdTexNormal)
{
	mdTexAlbedo_ = mdTexAlbedo;
	mdTexNormal_ = mdTexNormal;
}
MeshDependencies::~MeshDependencies()
{
	//Do nothing.
}

const LPCWSTR MeshDependencies::getMDTexAlbedo()
{
	return mdTexAlbedo_;
}
const LPCWSTR MeshDependencies::getMSDTexNormal()
{
	return mdTexNormal_;
}