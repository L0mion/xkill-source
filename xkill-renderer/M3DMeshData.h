#ifndef XKILL_RENDERER_M3DMESHDATA_H
#define XKILL_RENDERER_M3DMESHDATA_H

#include <DirectXMath.h>
#include <string>

struct M3DMaterial
{
	DirectX::XMFLOAT3	ambient_;
	DirectX::XMFLOAT3	diffuse_;
	DirectX::XMFLOAT4	specular_;
	DirectX::XMFLOAT3	reflectivity_;
	bool				alphaClip_;

	std::string			effectTypeName;
	std::string			diffuseMapName;
	std::string			normalMapName;

	M3DMaterial()
	{
		std::memset(this, 0, sizeof(M3DMaterial));
	}
};

struct M3DSubset
{
	unsigned int id_;
	unsigned int vertexStart_;
	unsigned int vertexCount_;
	unsigned int faceStart_;
	unsigned int faceCount_;

	M3DSubset()
	{
		std::memset(this, 0, sizeof(M3DSubset));
		id_ = -1;
	}
};

#endif //XKILL_RENDERER_M3DMESHDATA_H