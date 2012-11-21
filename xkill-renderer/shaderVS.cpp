#include <renderingUtilities.h>

#include <shaderVS.h>

ShaderVS::ShaderVS()
{
	vertexShader = nullptr;
}

ShaderVS::~ShaderVS()
{
	SAFE_RELEASE(vertexShader);
}

void ShaderVS::reset()
{
	Shader::reset();

	SAFE_RELEASE(vertexShader);
}

HRESULT ShaderVS::init(ID3D11Device* device, LPCWSTR shaderPath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(shaderPath, &blob);

	std::wstring location = L"ShaderVS::init D3DReadFileToBlob ";
	std::wstring failed = L" Failed!";
	std::wstring errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
	if(FAILED(hr))
		ERROR_MSG(errorMsg.c_str());
	else
	{
		hr = device->CreateVertexShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&vertexShader);
		
		location = L"ShaderVS::init CreateVertexShader ";
		errorMsg = location + static_cast<std::wstring>(shaderPath) + failed;
		if(FAILED(hr))
			ERROR_MSG(errorMsg.c_str());
	}

	return hr;
}

ID3D11VertexShader* ShaderVS::getVertexShader() const
{
	return vertexShader;
}