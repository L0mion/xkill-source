#include "renderingUtilities.h"
#include "ShaderGS.h"

ShaderGS::ShaderGS()
{
	geometryShader_ = nullptr;
}
ShaderGS::~ShaderGS()
{
	SAFE_RELEASE(geometryShader_);
}

void ShaderGS::reset()
{
	Shader::reset();

	SAFE_RELEASE(geometryShader_);
}
HRESULT ShaderGS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob_);

	std::wstring location = L"ShaderGS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreateGeometryShader(
			blob_->GetBufferPointer(),
			blob_->GetBufferSize(),
			nullptr,
			&geometryShader_);

		location = L"ShaderVS::init CreateGeometryShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}
void ShaderGS::set(ID3D11DeviceContext* devcon)
{
	devcon->GSSetShader(geometryShader_, nullptr, 0);
}

ID3D11GeometryShader* ShaderGS::getGeometryShader()
{
	return geometryShader_;
}