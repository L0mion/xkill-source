#ifndef XKILL_RENDERER_MESHVERTICES_H
#define XKILL_RENDERER_MESHVERTICES_H

#include "Math.h"

#include "dllUtilities.h"

enum DLL_U VertexType
{
	VERTEX_TYPE_POS_COLOR,
	VERTEX_TYPE_POS_NORM_TEX,
	VERTEX_TYPE_POS_NORM_SKINNED,
	VERTEX_TYPE_POS_NORM_TEX_SKINNED,
	VERTEX_TYPE_POS_NORM_TEX_TAN_SKINNED,
	VERTEX_TYPE_POS_NORM_TEX_TAN,

	VERTEX_INVALID
};

struct DLL_U VertexPosColor
{
	Float3 position_;
	Float3 color_;

	VertexPosColor();
	VertexPosColor(Float3 position, Float3 color);
};

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
	VertexPosNormTex();
	//! Constructor setting members to specific values.
	/*!
	\param position	Position in model-space of vertex.
	\param normal	Normal in model-space of vertex.
	\param texcoord	Texture-coordinates.
	*/
	VertexPosNormTex(
		Float3 position, 
		Float3 normal, 
		Float2 texcoord);
};


static const unsigned int NUM_BONES_PER_VERTEX = 4; 

//! Vertex for a skinned mesh.
/*!
\ingroup xkill-mesh*/
struct DLL_U VertexPosNormSkinned
{
	Float3 position_;								 //!< Position in model-space.
	Float3 normal_;									 //!< Normal in model-space.
	Float3 weights_;								 //!< How much each bone influences the vertex.
	unsigned int boneIndices_[NUM_BONES_PER_VERTEX]; //!< Which bones that influence the vertex.

	//! Default constructor setting all member variables to zero.
	VertexPosNormSkinned();
	//! Constructor setting members to specific values.
	/*!
	\param position Position in model-space.
	\patam normal Normal in model-space.
	\param weights How much each bone influences the vertex.
	\param boneIndices[] Array containing indices for each bone influensing the vertex. Max 4!
	*/
	VertexPosNormSkinned(Float3 position, Float3 normal, Float3 weights, unsigned int boneIndices[]);
};

//! Vertex for a skinned mesh.
/*!
\ingroup xkill-mesh*/
struct DLL_U VertexPosNormTexSkinned
{
	Float3 position_;								 //!< Position in model-space.
	Float3 normal_;									 //!< Normal in model-space.
	Float2 texcoord_;								 //!< Texture coordinate.
	Float3 weights_;								 //!< How much each bone influences the vertex.
	unsigned int boneIndices_[NUM_BONES_PER_VERTEX]; //!< Which bones that influence the vertex.

	//! Default constructor setting all member variables to zero.
	VertexPosNormTexSkinned();
	//! Constructor setting members to specific values.
	/*!
	\param position Position in model-space.
	\patam normal Normal in model-space.
	\param texcoord Texture coordinates.
	\param weights How much each bone influences the vertex.
	\param boneIndices[] Array containing indices for each bone influensing the vertex. Max 4!
	*/
	VertexPosNormTexSkinned(Float3 position,
							Float3 normal,
							Float2 texcoord,
							Float3 weights,
							unsigned int boneIndices[]);
};


//! Vertex for a skinned mesh.
/*!
\ingroup xkill-mesh*/
struct DLL_U VertexPosNormTexTanSkinned
{
	Float3		 position_;							 //!< Position in model-space.
	Float3		 normal_;							 //!< Normal in model-space.
	Float2		 texcoord_;							 //!< Texture coordinates.
	Float4		 tangent_;							 //!< Tangent in model-space.
	Float3		 weights_;							 //!< How much each bone influences the vertex.
	unsigned int boneIndices_[NUM_BONES_PER_VERTEX]; //!< Indices of the bones influencing the vertex.

	//! Default constructor setting all member variables to zero.
	VertexPosNormTexTanSkinned()
	{
		position_	= Float3(0.0f, 0.0f, 0.0f);
		normal_		= Float3(0.0f, 0.0f, 0.0f);
		texcoord_	= Float2(0.0f, 0.0f);
		tangent_	= Float4(0.0f, 0.0f, 0.0f, 0.0f);
		weights_	= Float3(0.0f, 0.0f, 0.0f);
		for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
		boneIndices_[i] = 0;
	}
	//! Constructor setting members to specific values.
	/*!
	\param position Position in model-space.
	\patam normal Normal in model-space.
	\param texcoord Texture coordinates.
	\param tangent Tangent in model-space.
	\param weights How much each bone influences the vertex.
	\param boneIndices[] Array containing indices for each bone influensing the vertex. Max 4!
	*/
	VertexPosNormTexTanSkinned(Float3 position, Float3 normal, Float2 texcoord,	Float4 tangent, Float3 weights, unsigned int boneIndices[])
	{
		position_	= position;
		normal_		= normal;
		texcoord_	= texcoord;
		tangent_	= tangent;
		weights_	= weights;
		for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
		boneIndices_[i] = boneIndices[i];
	}
};

struct DLL_U VertexPosNormTexTan
{
	Float3		 position_;							 //!< Position in model-space.
	Float3		 normal_;							 //!< Normal in model-space.
	Float2		 texcoord_;							 //!< Texture coordinates.
	Float4		 tangent_;							 //!< Tangent in model-space.

	//! Default constructor setting all member variables to zero.
	VertexPosNormTexTan()
	{
		position_	= Float3(0.0f, 0.0f, 0.0f);
		normal_		= Float3(0.0f, 0.0f, 0.0f);
		texcoord_	= Float2(0.0f, 0.0f);
		tangent_	= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	//! Constructor setting members to specific values.
	/*!
	\param position Position in model-space.
	\patam normal Normal in model-space.
	\param texcoord Texture coordinates.
	\param tangent Tangent in model-space.
	*/
	VertexPosNormTexTan(Float3 position, Float3 normal, Float2 texcoord,	Float4 tangent)
	{
		position_	= position;
		normal_		= normal;
		texcoord_	= texcoord;
		tangent_	= tangent;
	}
};

#endif //XKILL_RENDERER_MESHVERTICES_H

//DirectX::XMFLOAT3 dxfloat((float*)&position_);