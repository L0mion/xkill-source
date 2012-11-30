#ifndef XKILL_RENDERER_MTL_H
#define XKILL_RENDERER_MTL_H

#include <vector>

class MTLMaterial;

class MTL
{
public:
	MTL();
	MTL(std::vector<MTLMaterial> materials);
	~MTL();

	const std::vector<MTLMaterial> getMaterials();
protected:
private:
	std::vector<MTLMaterial> materials_;
};

#endif //XKILL_RENDERER_MTL_H