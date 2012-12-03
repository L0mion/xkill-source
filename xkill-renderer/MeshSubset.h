#ifndef XKILL_RENDERER_MESHSUBSET_H
#define XKILL_RENDERER_MESHSUBSET_H

#include <vector>
#include <string>

class MeshSubset
{
public:
	MeshSubset();
	MeshSubset(
		const unsigned int materialIndex,
		const std::vector<unsigned int> indices);
	~MeshSubset();

	const unsigned int getMaterialIndex();
	const unsigned int getNumIndices();
	const std::vector<unsigned int> getIndices();
protected:
private:
	unsigned int materialIndex_;
	std::vector<unsigned int> indices_;
};

#endif //XKILL_RENDERER_MESHSUBSET_H