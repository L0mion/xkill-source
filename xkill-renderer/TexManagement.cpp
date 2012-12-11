#include <Windows.h>
#include <string>
#include <vector>
#include <DDSTextureLoader.h>
#include <d3d11.h>

#include <xkill-utilities/TexDesc.h>
#include <xkill-utilities/TexDescHeader.h>
#include <xkill-utilities/TexDescTex.h>

#include "TexManagement.h"

TexManagement::TexManagement()
{
	//Do nothing.
}
TexManagement::~TexManagement()
{
	//Do nothing.
}

HRESULT TexManagement::init()
{
	HRESULT hr = S_OK;

	//Do nothing.

	return hr;
}

HRESULT TexManagement::handleTexDesc(
	TexDesc*		texDesc,
	ID3D11Device*	device)
{
	std::string				texPath		= texDesc->getHeader().texPath_;
	std::vector<TexDescTex>	texDescs	= texDesc->getTexDescs();

	unsigned int	texID;
	std::string		texFileName;
	for(unsigned int i = 0; i < texDescs.size(); i++)
	{
		texID		= texDescs.at(i).id_;
		texFileName	= texDescs.at(i).fileName_;

		createTex(
			texID,
			texPath,
			texFileName,
			device);
	}
}
HRESULT TexManagement::createTex(
		unsigned int	texID, 
		std::string		texPath, 
		std::string		texFileName,
		ID3D11Device*	device)
{
	HRESULT hr = S_OK;

	std::string fullPath = texPath + texFileName;

	int wchars_num =  MultiByteToWideChar( CP_UTF8 , 0 , fullPath.c_str() , -1, NULL , 0 );
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar( CP_UTF8 , 0 , fullPath.c_str() , -1, wstr , wchars_num );

	ID3D11ShaderResourceView* srv = nullptr;
	hr = DirectX::CreateDDSTextureFromFile(
		device,
		wstr, //L"SEAFLOOR.DDS"
		nullptr,
		&srv);
	
	delete[] wstr;

	return hr;
}