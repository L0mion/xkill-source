#include <renderingUtilities.h>

#include <shaderPS.h>

ShaderPS::ShaderPS()
{
	pixelShader = nullptr;
}

ShaderPS::~ShaderPS()
{
	SAFE_RELEASE(pixelShader);
}

void ShaderPS::reset()
{
	Shader::reset();

	SAFE_RELEASE(pixelShader);
}

HRESULT ShaderPS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob);

	std::wstring location = L"ShaderPS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreatePixelShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&pixelShader);
		
		location = L"ShaderVS::init CreateVertexShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

ID3D11PixelShader* ShaderPS::getPixelShader()
{
	return pixelShader;
}