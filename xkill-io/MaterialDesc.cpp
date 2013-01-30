#include "MaterialDesc.h"

MaterialDesc::MaterialDesc()
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
MaterialDesc::~MaterialDesc()
{
	//Do nothing.
}