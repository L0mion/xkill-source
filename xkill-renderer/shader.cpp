#include <renderingUtilities.h>

#include <shader.h>

Shader::Shader()
{
	shaderPath	= L"Default";
	blob		= nullptr;
}
Shader::~Shader()
{
	SAFE_RELEASE(blob);
}

void Shader::reset()
{
	shaderPath = L"Default";

	SAFE_RELEASE(blob);
}

ID3D10Blob* Shader::getBlob()
{
	return blob;
}