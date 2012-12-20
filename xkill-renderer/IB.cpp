#include <d3d11.h>

#include "renderingUtilities.h"
#include "IB.h"

IB::IB()
{
	ib_				= nullptr;
	numIndices_		= 0;
}
IB::~IB()
{
	SAFE_RELEASE(ib_);
}

HRESULT IB::init(
	std::vector<unsigned int>	indices,
	ID3D11Device*				device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage			= D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth		= sizeof(unsigned int) * indices.size();
	ibd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;

	if(indices.size() <= 0)
		return E_FAIL;

	vinitData.pSysMem = &indices.at(0);

	hr = device->CreateBuffer(&ibd, &vinitData, &ib_);
	numIndices_ = indices.size();

	return hr;
}

ID3D11Buffer*		IB::getIB()
{
	return ib_;
}
const unsigned int	IB::getNumIndices()
{
	return numIndices_;
}