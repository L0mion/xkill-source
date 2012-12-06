#include <d3d11.h>

#include <xkill-utilities\MeshVertices.h>

#include "renderingUtilities.h"
#include "VB.h"

VB::VB()
{
	vb_				= nullptr;
	numVertices_	= 0;
}
VB::~VB()
{
	SAFE_RELEASE(vb_);
}

HRESULT VB::init(
	std::vector<VertexPosNormTex>&	vertices,
	ID3D11Device*					device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage			= D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth		= sizeof(VertexPosNormTex) * vertices.size();
	vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices.at(0);

	hr = device->CreateBuffer(&vbd, &vinitData, &vb_);
	numVertices_ = vertices.size();

	return hr;
}

ID3D11Buffer*		VB::getVB()
{
	return vb_;
}
const unsigned int	VB::getNumVertices()
{
	return numVertices_;
}