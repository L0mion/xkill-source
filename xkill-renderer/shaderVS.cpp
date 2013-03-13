#include "renderingUtilities.h"

#include "shaderVS.h"

ShaderVS::ShaderVS()
{
	vertexShader_ = nullptr;
}

ShaderVS::~ShaderVS()
{
	SAFE_RELEASE(vertexShader_);
}

void ShaderVS::reset()
{
	Shader::reset();

	SAFE_RELEASE(vertexShader_);
}

HRESULT ShaderVS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob_);

	std::wstring location = L"ShaderVS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreateVertexShader(
			blob_->GetBufferPointer(),
			blob_->GetBufferSize(),
			nullptr,
			&vertexShader_);
		
		location = L"ShaderVS::init CreateVertexShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

void ShaderVS::set(ID3D11DeviceContext* devcon)
{
	devcon->VSSetShader(vertexShader_, nullptr, 0);
}

ID3D11VertexShader* ShaderVS::getVertexShader() const
{
	return vertexShader_;
}