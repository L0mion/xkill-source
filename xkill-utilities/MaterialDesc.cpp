#include "MaterialDesc.h"

MaterialDesc::MaterialDesc()
{
	ambientTerm_	= Float3(0.2f, 0.2f, 2.0f);
	diffuseTerm_	= Float3(0.2f, 0.2f, 2.0f);
	specularTerm_	= Float3(0.2f, 0.2f, 2.0f);
	specularPower_	= 1.0f;
	reflectivity_	= Float3(0.5f, 0.5f, 0.5f);
	transperency_	= false;

	emissive_	= Float3(0.0f, 0.0f, 0.0f);
	shinyness_	= 0.0f;

	idAlbedoTex_ = 0;
	idNormalTex_ = 0;
}
MaterialDesc::~MaterialDesc()
{
	//Do nothing.
}