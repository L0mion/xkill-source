#include "renderingUtilities.h"

#include "shaderCS.h"


ShaderCS::ShaderCS()
{
	computeShader = nullptr;
}

ShaderCS::~ShaderCS()
{
	SAFE_RELEASE(computeShader);
}

void ShaderCS::reset()
{
	Shader::reset();

	SAFE_RELEASE(computeShader);
}

HRESULT ShaderCS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob);

	std::wstring location = L"ShaderCS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreateComputeShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&computeShader);
		
		location = L"ShaderCS::init CreateComputeShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

void ShaderCS::set(ID3D11DeviceContext* devcon)
{
	devcon->CSSetShader(computeShader, nullptr, 0);
}
void ShaderCS::unset(ID3D11DeviceContext* devcon)
{
	devcon->CSSetShader(nullptr, nullptr, 0);
}

ID3D11ComputeShader* ShaderCS::getComputeShader() const
{
	return computeShader;
}