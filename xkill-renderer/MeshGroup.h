#ifndef XKILL_RENDERER_MESHGROUP_H
#define XKILL_RENDERER_MESHGROUP_H

#include <vector>

class MeshGroup
{
public:
	MeshGroup(const std::string mgName);
	~MeshGroup();

	void mgPushIndex(unsigned int index);

	bool isValid();

	void setMGMaterial(std::string mgMaterial);

	const std::string				getMGName();
	const std::string				getMGMaterial();
	const unsigned int				getMGNumIndices();
	const std::vector<unsigned int>	getMGIndices();
protected:
private:
	std::string					mgName_;
	std::string					mgMaterial_;
	std::vector<unsigned int>	mgIndices_;
};

#endif //XKILL_RENDERER_MESHGROUP_H