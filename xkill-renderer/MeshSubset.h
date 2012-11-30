#ifndef XKILL_RENDERER_MESHSUBSET_H
#define XKILL_RENDERER_MESHSUBSET_H

#include <vector>
#include <string>

class MeshSubset
{
public:
	MeshSubset(
		const std::string name,
		const std::string materialName,
		const std::vector<unsigned int> indices);
	~MeshSubset();

	const std::string getName();
	const std::string getMaterialName();
	const std::vector<unsigned int> getIndices();
protected:
private:
	std::string name_;
	std::string materialName_;
	std::vector<unsigned int> indices_;
};

#endif //XKILL_RENDERER_MESHSUBSET_H