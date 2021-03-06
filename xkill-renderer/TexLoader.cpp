#include "TexLoader.h"

#include <xkill-utilities/StringConv.h>
#include "DDSTextureLoader.h"

TexLoader::TexLoader()
{
	//Do nothing.
}
TexLoader::~TexLoader()
{
	//Do nothing.
}

HRESULT TexLoader::createTexFromFile(
		ID3D11Device*				device,
		std::string					file,
		ID3D11ShaderResourceView**	srv)
{
	HRESULT hr = S_OK;

	wchar_t* wstr = stringToWstr(file);
	hr = DirectX::CreateDDSTextureFromFile(
		device,
		wstr,
		nullptr,
		srv);

	delete[] wstr;
	return hr;
}