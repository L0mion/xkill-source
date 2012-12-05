#ifndef XKILL_RENDERER_MESHVERTICES_H
#define XKILL_RENDERER_MESHVERTICES_H

#include "Math.h"

#include "dllUtilities.h"

//! Basic three-member vertex
/*!
\ingroup xkill-mesh*/
struct DLL_U VertexPosNormTex
{
	Float3 position_;	//!< Float3 vector specifying position in model-space of vertex.
	Float3 normal_;		//!< Float3 vector specifying normal in model-space of vertex.
	Float2 texcoord_;	//!< Float3 vector specifying texture-coordinates.

	//! Default constructor of Vertex.
	/*!
	Initializes Vertex to default values.
	position	= 0, 0, 0
	normal		= 0, 0, 0
	texcoord	= 0, 0
	*/
	VertexPosNormTex()
	{
		position_	= Float3(0.0f, 0.0f, 0.0f);
		normal_		= Float3(0.0f, 0.0f, 0.0f);
		texcoord_	= Float2(0.0f, 0.0f);
	}
	//! Constructor setting members to specific values.
	/*!
	\param position	Position in model-space of vertex.
	\param normal	Normal in model-space of vertex.
	\param texcoord	Texture-coordinates.
	*/
	VertexPosNormTex(
		Float3 position, 
		Float3 normal, 
		Float2 texcoord)
	{
		position_	= position;
		normal_		= normal;
		texcoord_	= texcoord;
	}
};

#endif //XKILL_RENDERER_MESHVERTICES_H

//DirectX::XMFLOAT3 dxfloat((float*)&position_);