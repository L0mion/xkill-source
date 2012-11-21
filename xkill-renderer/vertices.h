#ifndef XKILL_RENDERER_VERTEX_H
#define XKILL_RENDERER_VERTEX_H

/*! 
	\defgroup xkill-renderer-vertices xkill-renderer-vertices
	\ingroup xkill-renderer
	Various types of Vertices used by xkill-renderer
*/

#include "mathBasic.h"

//! Basic three-member vertex
/*!
\ingroup xkill-renderer-vertices*/
struct Vertex
{
	VecF3 position;	//!< float3 vector specifying position in model-space of vertex.
	VecF3 normal;	//!< float3 vector specifying normal in model-space of vertex.
	VecF2 texcoord;	//!< float3 vector specifying texture-coordinates.

	//! Default constructor of Vertex.
	/*!
	Initializes Vertex to default values.
		position	= 0, 0, 0
		normal		= 0, 0, 0
		texcoord	= 0, 0
	*/
	Vertex()
	{
		this->position	= VecF3(0.0f, 0.0f, 0.0f);
		this->normal	= VecF3(0.0f, 0.0f, 0.0f);
		this->texcoord	= VecF2(0.0f, 0.0f);
	}
	//! Constructor setting members to specific values.
	/*!
	\param position	Position in model-space of vertex.
	\param normal	Normal in model-space of vertex.
	\param texcoord	Texture-coordinates.
	*/
	Vertex(VecF3 position, VecF3 normal, VecF2 texcoord)
	{
		this->position	= position;
		this->normal	= normal;
		this->texcoord	= texcoord;
	}
};

#endif //XKILL_RENDERER_VERTEX_H