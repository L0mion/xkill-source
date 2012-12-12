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

	std::string			effectTypeName_;
	std::string			diffuseMapName_;
	std::string			normalMapName_;

	M3DMaterial()
	{
		ambient_		= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		diffuse_		= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		specular_		= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		reflectivity_	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		alphaClip_		= false;
		
		effectTypeName_ = "Unknown";
		diffuseMapName_ = "Unknown";
		normalMapName_	= "Unkown";
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
		id_ = -1;
		vertexStart_ = 0;
		vertexCount_ = 0;
		faceStart_	 = 0;
		faceCount_	 = 0;
	}
};

#endif //XKILL_RENDERER_M3DMESHDATA_H