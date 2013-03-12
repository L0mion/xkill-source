#ifndef XKILL_IO_LOADER_MD5_MESH_DESC_H
#define XKILL_IO_LOADER_MD5_MESH_DESC_H

#include <vector>
#include <string>

#include <DirectXMath.h>

#include "LoaderMD5TriangleDesc.h"
#include "LoaderMD5WeightDesc.h"
#include "LoaderMD5VertexDesc.h"

struct LoaderMD5MeshDesc
{
	std::string shader_;

	std::vector<LoaderMD5VertexDesc>	vertices_;
	std::vector<LoaderMD5TriangleDesc>	triangles_;
	std::vector<LoaderMD5WeightDesc>	weights_;

	unsigned int textureID_;

	std::vector<DirectX::XMFLOAT3>	positions_;
	std::vector<DirectX::XMFLOAT3>	normals_;
	std::vector<DirectX::XMFLOAT2>	texcoords_;
	std::vector<unsigned int>		indices_;

	LoaderMD5MeshDesc()
	{
		shader_		= "Unknown";
		textureID_	= -1;
	}
};

#endif //XKILL_IO_LOADER_MD5_MESH_DESC_H