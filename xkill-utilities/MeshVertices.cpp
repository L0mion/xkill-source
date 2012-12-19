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