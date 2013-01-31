#ifndef XKILL_IO_MESHDESC_H
#define XKILL_IO_MESHDESC_H

#include <vector>

#include "VertexDesc.h"
#include "MaterialDesc.h"
#include "SubsetDesc.h"

class DLL_U MeshDesc
{
public:
	MeshDesc();
	~MeshDesc();
protected:
public:
	std::vector<VertexDesc> vertices_;
	std::vector<SubsetDesc> subsets_;
	std::vector<MaterialDesc> materials_;
};

#endif //XKILL_IO_MESHDESC_H