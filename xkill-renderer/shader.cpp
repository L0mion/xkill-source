#include <d3dcompiler.h>

#include "renderingUtilities.h"
#include "shader.h"

Shader::Shader()
{
	shaderPath_	= L"Default";
	blob_		= nullptr;
}
Shader::~Shader()
{
	SAFE_RELEASE(blob_);
}

void Shader::reset()
{
	shaderPath_ = L"Default";

	SAFE_RELEASE(blob_);
}

ID3D10Blob* Shader::getBlob()
{
	return blob_;
}