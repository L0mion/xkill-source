#ifndef XKILL_RENDERER_VB_H
#define XKILL_RENDERER_VB_H

//typedef long HRESULT;
//
//struct ID3D11Buffer;
//struct VertexPosNormTex;
//struct ID3D11Device;

#include <d3d11.h>
#include <vector>

#include "renderingUtilities.h"

//! Describes a vertex buffer.
/*!
\ingroup xkill-renderer
*/
template <class Vertex>
class VB
{
public:
	VB()	//!< Initializes VB to default state.
	{
		vb_				= nullptr;
		numVertices_	= 0;
	}
	~VB()	//!< Release ID3D11Buffer-type object.
	{
		SAFE_RELEASE(vb_);
	}

	HRESULT init(
		std::vector<Vertex>&	vertices,
		ID3D11Device*			device)	//!< Initializes VB based on vertices sent as argument.
	{
		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage			= D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth		= sizeof(Vertex) * vertices.size();
		vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags		= 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices.at(0);

		hr = device->CreateBuffer(&vbd, &vinitData, &vb_);
		numVertices_ = vertices.size();

		return hr;
	}

	ID3D11Buffer*		getVB()				{ return vb_;			}
	const unsigned int	getNumVertices()	{ return numVertices_;	}
protected:
private:
	ID3D11Buffer*	vb_;			//!< D3D-vertex buffer type object.
	unsigned int	numVertices_;	//!< Number of vertices contained in vertex buffer.
};

#endif //XKILL_RENDERER_VB_H