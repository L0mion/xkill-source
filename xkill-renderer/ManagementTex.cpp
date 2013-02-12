#include <d3d11.h>
#include "DDSTextureLoader.h"

#include <xkill-utilities/Util.h>
#include <xkill-utilities/TexDesc.h>
#include <xkill-utilities/TexDescHeader.h>
#include <xkill-utilities/TexDescTex.h>
#include <xkill-utilities/StringConv.h>

#include "Tex.h"
#include "TexLoader.h"
#include "ManagementTex.h"

ManagementTex::ManagementTex()
{
	//Do nothing.
}
ManagementTex::~ManagementTex()
{
	if(texLoader_)
		delete texLoader_;

	for(unsigned int i = 0; i < texs_.size(); i++)
	{
		if(texs_[i])
			delete texs_[i];
	}
}

HRESULT ManagementTex::init()
{
	HRESULT hr = S_OK;

	texLoader_ = new TexLoader();

	return hr;
}

HRESULT ManagementTex::handleTexDesc(
	TexDesc*		texDesc,
	ID3D11Device*	device)
{
	HRESULT hr = S_OK;

	std::string	path = texDesc->getPath() + texDesc->getHeader().texPath_;
	std::vector<TexDescTex>	texDescs = texDesc->getTexDescs();

	unsigned int	texID;
	std::string		texFileName;
	for(unsigned int i = 0; i < texDescs.size(); i++)
	{
		texID		= texDescs.at(i).id_;
		texFileName	= texDescs.at(i).fileName_;

		hr = createTex(
			texID,
			path,
			texFileName,
			device);
		if(FAILED(hr))
		{
			SHOW_MESSAGEBOX("ManagementTex::handleTexDesc Could not load texture: " + texFileName);
		}
	}

	return hr;
}
HRESULT ManagementTex::createTex(
		unsigned int	texID, 
		std::string		texPath, 
		std::string		texFileName,
		ID3D11Device*	device)
{
	HRESULT hr = S_OK;

	std::string fullPath = texPath + texFileName;

	ID3D11ShaderResourceView* srv = nullptr;
	hr = texLoader_->createTexFromFile(
		device,
		fullPath,
		&srv);

	if(!FAILED(hr))
		pushTex(texID, srv);
	
	return hr;
}
void ManagementTex::pushTex(
		unsigned int				texID,
		ID3D11ShaderResourceView*	srv)
{
	/*Create new Tex-object*/
	Tex* newTex = new Tex(texID, srv);

	texs_.push_back(newTex);
	unsigned int texsIndex = texs_.size() - 1;

	/*Map index of pushed back element to texture ID*/
	std::pair<
		std::map<unsigned int, unsigned int>::iterator,
		bool> ret;
	ret = texIDtoIndex_.insert(
		std::pair<unsigned int, unsigned int>(texID, texsIndex));

	if(!ret.second)
	{ //Element already exists in vector
		std::string errorMsg = 
			"TextureID "									+ 
			uintToString(texID)								+ 
			" already exists in TexManagement under Index " + 
			uintToString(texsIndex)							+ 
			".";
		SHOW_MESSAGEBOX(errorMsg);
	}
}

ID3D11ShaderResourceView* ManagementTex::getTexSrv(const unsigned int texID) const
{
	ID3D11ShaderResourceView* srv = nullptr;

	int texsIndex = getTexsIndex(texID);
	if(texsIndex >= 0)
		srv = texs_[static_cast<unsigned>(texsIndex)]->getSRV();

	return srv;
}
int ManagementTex::getTexsIndex(const unsigned int texID) const
{
	int foundIndex = -1;

	std::map<unsigned int, unsigned int>::const_iterator it;
	it = texIDtoIndex_.find(texID);
	if(it != texIDtoIndex_.end())
		foundIndex = it->second;

	return foundIndex;
}