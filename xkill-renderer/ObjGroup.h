#ifndef XKILL_RENDERER_OBJGROUP_H
#define XKILL_RENDERER_OBJGROUP_H

#include <vector>

class ObjGroup
{
public:
	ObjGroup(const std::string mgName);
	~ObjGroup();

	void pushIndex(unsigned int index);

	void setMaterialName(std::string materialName);

	const std::string				getName();
	const std::string				getMaterial();
	const unsigned int				getNumIndices();
	const std::vector<unsigned int>	getIndices();
protected:
private:
	std::string					name_;
	std::string					materialName_;
	std::vector<unsigned int>	indices_;
};

#endif //XKILL_RENDERER_OBJGROUP_H