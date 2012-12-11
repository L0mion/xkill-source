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

//! Vertex for a skinned mesh.
/*!
\ingroup xkill-mesh*/
struct DLL_U VertexPosNormTexSkinned
{
	Float3		 position_;			//!< Position in model-space.
	Float3		 normal_;			//!< Normal in model-space.
	Float2		 texcoord_;			//!< Texture coordinates.
	Float3		 weights_;			//!< How much each bone influences the vertex.
	unsigned int boneIndices_[4];	//!< Indices of the bones influencing the vertex.

	//! Default constructor setting all member variables to zero.
	VertexPosNormTexSkinned()
	{
		std::memset(this, 0, sizeof(VertexPosNormTexSkinned));
	}

	//! Constructor setting members to specific values.
	/*!
	\param position Position in model-space.
	\patam normal Normal in model-space.
	\param texcoord Texture coordinates.
	\param weights How much each bone influences the vertex.
	\param boneIndices[] Array containing indices for each bone influensing the vertex. Max 4!
	*/
	VertexPosNormTexSkinned(Float3 position, Float3 normal, Float2 texcoord, Float3 weights, unsigned int boneIndices[])
	{
		position_ = position;
		normal_ = normal;
		texcoord_ = texcoord;
		weights_ = weights;
		for(unsigned int i=0; i<4; i++)
			boneIndices_[i] = boneIndices[i];
	}
};

#endif //XKILL_RENDERER_MESHVERTICES_H

//DirectX::XMFLOAT3 dxfloat((float*)&position_);