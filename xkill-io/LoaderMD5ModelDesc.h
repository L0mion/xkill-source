#ifndef XKILL_IO_LOADER_MD5_MODEL_DESC_H
#define XKILL_IO_LOADER_MD5_MODEL_DESC_H

#include <vector>

#include "LoaderMD5JointDesc.h"
#include "LoaderMD5MeshDesc.h"

struct LoaderMD5ModelDesc
{
	std::vector<LoaderMD5JointDesc> joints_;
	std::vector<LoaderMD5MeshDesc> meshes_;
};

#endif //XKILL_IO_LOADER_MD5_MODEL_DESC_H