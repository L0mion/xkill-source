#ifndef XKILL_RENDERER_VERTEX_H
#define XKILL_RENDERER_VERTEX_H

/*! 
	\defgroup xkill-renderer-vertices xkill-renderer-vertices
	\ingroup xkill-renderer
	Various types of Vertices used by xkill-renderer
*/

#include <DirectXMath.h>

//! Basic three-member vertex
/*!
\ingroup xkill-renderer-vertices*/
struct VertexPosNormTex
{
	DirectX::XMFLOAT3 position_;	//!< float3 vector specifying position in model-space of vertex.
	DirectX::XMFLOAT3 normal_;	//!< float3 vector specifying normal in model-space of vertex.
	DirectX::XMFLOAT2 texcoord_;	//!< float3 vector specifying texture-coordinates.

	//! Default constructor of Vertex.
	/*!
	Initializes Vertex to default values.
		position	= 0, 0, 0
		normal		= 0, 0, 0
		texcoord	= 0, 0
	*/
	VertexPosNormTex()
	{
		this->position_	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->normal_	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->texcoord_	= DirectX::XMFLOAT2(0.0f, 0.0f);
	}
	//! Constructor setting members to specific values.
	/*!
	\param position	Position in model-space of vertex.
	\param normal	Normal in model-space of vertex.
	\param texcoord	Texture-coordinates.
	*/
	VertexPosNormTex(
		DirectX::XMFLOAT3 position, 
		DirectX::XMFLOAT3 normal, 
		DirectX::XMFLOAT2 texcoord)
	{
		this->position_	= position;
		this->normal_	= normal;
		this->texcoord_	= texcoord;
	}
};

#endif //XKILL_RENDERER_VERTEX_H