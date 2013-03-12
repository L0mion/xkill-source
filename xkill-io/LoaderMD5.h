#ifndef XKILL_IO_LOADER_MD5_H
#define XKILL_IO_LOADER_MD5_H

#include <vector>
#include <fstream>

#include <DirectXMath.h>

#include "LoaderMD5MeshDesc.h"
#include "LoaderMD5ModelDesc.h"
#include "LoaderMD5AnimationDesc.h"

class LoaderMD5
{
public:
	LoaderMD5();
	~LoaderMD5();

	bool loadModel(const std::string& filename, LoaderMD5ModelDesc* modelDesc);
	bool loadAnimation(const std::string& filename, LoaderMD5AnimationDesc* animationDesc);

private:
	

	DirectX::XMFLOAT4X4 localToWorldMatrix_;
};

#endif //XKILL_IO_LOADER_MD5_H