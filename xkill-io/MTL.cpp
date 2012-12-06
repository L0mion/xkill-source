#include "MTLMaterial.h"

#include "MTL.h"

MTL::MTL()
{
	//Do nothing
}
MTL::MTL(const std::vector<MTLMaterial> materials)
{
	materials_ = materials;
}
MTL::~MTL()
{
	//Do nothing.
}

const std::vector<MTLMaterial> MTL::getMaterials()
{
	return materials_;
}