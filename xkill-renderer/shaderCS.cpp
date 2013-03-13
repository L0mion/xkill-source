#include "renderingUtilities.h"

#include "shaderCS.h"


ShaderCS::ShaderCS()
{
	computeShader_ = nullptr;
}

ShaderCS::~ShaderCS()
{
	SAFE_RELEASE(computeShader_);
}

void ShaderCS::reset()
{
	Shader::reset();

	SAFE_RELEASE(computeShader_);
}

HRESULT ShaderCS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob_);

	std::wstring location = L"ShaderCS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreateComputeShader(
			blob_->GetBufferPointer(),
			blob_->GetBufferSize(),
			nullptr,
			&computeShader_);
		
		location = L"ShaderCS::init CreateComputeShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

void ShaderCS::set(ID3D11DeviceContext* devcon)
{
	devcon->CSSetShader(computeShader_, nullptr, 0);
}

ID3D11ComputeShader* ShaderCS::getComputeShader() const
{
	return computeShader_;
}