#ifndef XKILL_IO_MESHDESC_H
#define XKILL_IO_MESHDESC_H

#include <vector>

#include "VertexDesc.h"
#include "MaterialDesc.h"
#include "SubsetDesc.h"

#include "SkinnedData.h"

class DLL_U MeshDesc
{
public:
	MeshDesc();
	~MeshDesc();

	bool hasAnimation();
protected:
public:
	std::vector<VertexDesc> vertices_;
	std::vector<SubsetDesc> subsets_;
	std::vector<MaterialDesc> materials_;

	SkinnedData* skinnedData_; 
};

#endif //XKILL_IO_MESHDESC_H