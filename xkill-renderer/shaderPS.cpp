#include "renderingUtilities.h"

#include "shaderPS.h"

ShaderPS::ShaderPS()
{
	pixelShader_ = nullptr;
}

ShaderPS::~ShaderPS()
{
	SAFE_RELEASE(pixelShader_);
}

void ShaderPS::reset()
{
	Shader::reset();

	SAFE_RELEASE(pixelShader_);
}

HRESULT ShaderPS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob_);

	std::wstring location = L"ShaderPS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreatePixelShader(
			blob_->GetBufferPointer(),
			blob_->GetBufferSize(),
			nullptr,
			&pixelShader_);
		
		location = L"ShaderVS::init CreatePixelShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

void ShaderPS::set(ID3D11DeviceContext* devcon)
{
	devcon->PSSetShader(pixelShader_, nullptr, 0);
}

ID3D11PixelShader* ShaderPS::getPixelShader()
{
	return pixelShader_;
}