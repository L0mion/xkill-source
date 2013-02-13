#include "MeshVertices.h"

VertexPosColor::VertexPosColor()
{
	position_	= Float3();
	color_		= Float3(1.0f, 1.0f, 1.0f);

}
VertexPosColor::VertexPosColor(Float3 position, Float3 color)
{
	position_	= position;
	color_		= color;
}

VertexPosNormTex::VertexPosNormTex()
{
	position_	= Float3(0.0f, 0.0f, 0.0f);
	normal_		= Float3(0.0f, 0.0f, 0.0f);
	texcoord_	= Float2(0.0f, 0.0f);
}
VertexPosNormTex::VertexPosNormTex(
	Float3 position, 
	Float3 normal, 
	Float2 texcoord)
{
	position_	= position;
	normal_		= normal;
	texcoord_	= texcoord;
}


VertexPosNormSkinned::VertexPosNormSkinned()
{
	position_	= Float3(0.0f, 0.0f, 0.0f);
	normal_		= Float3(0.0f, 0.0f, 0.0f);
	weights_	= Float3(0.0f, 0.0f, 0.0f);
	for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
		boneIndices_[i] = 0;
}
VertexPosNormSkinned::VertexPosNormSkinned(Float3 position, Float3 normal, Float3 weights, unsigned int boneIndices[])
{
	position_ = position;
	normal_ = normal;
	weights_ = weights;
	for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
		boneIndices_[i] = boneIndices[i];
}

VertexPosNormTexSkinned::VertexPosNormTexSkinned()
{
	position_	= Float3(0.0f, 0.0f, 0.0f);
	normal_		= Float3(0.0f, 0.0f, 0.0f);
	texcoord_	= Float2(0.0f, 0.0f);
	weights_	= Float3(0.0f, 0.0f, 0.0f);
	for(unsigned int i=0; i<NUM_BONES_PER_VERTEX; i++)
		boneIndices_[i] = 0;
}
VertexPosNormTexSkinned::VertexPosNormTexSkinned(Float3 position,
												 Float3 normal,
												 Float2 texcoord,
												 Float3 weights,
												 unsigned int boneIndices[])
{
	position_	= position;
	normal_		= normal;
	texcoord_	= texcoord;
	weights_	= weights;
	for(unsigned int i=0; i<4; i++)
		boneIndices_[i] = boneIndices[i];
}

VertexPosNormTexTan::VertexPosNormTexTan()
{
	position_	= Float3(0.0f, 0.0f, 0.0f);
	normal_		= Float3(0.0f, 0.0f, 0.0f);
	texcoord_	= Float2(0.0f, 0.0f);
	tangent_	= Float4(0.0f, 0.0f, 0.0f, 0.0f);
}
VertexPosNormTexTan::VertexPosNormTexTan(
	Float3 position, 
	Float3 normal, 
	Float2 texcoord,
	Float4 tangent)
{
	position_	= position;
	normal_		= normal;
	texcoord_	= texcoord;
	tangent_	= tangent;
}
