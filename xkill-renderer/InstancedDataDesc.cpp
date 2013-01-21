#include "InstancedDataDesc.h"

VertexPosNormTexInstanced::VertexPosNormTexInstanced()
{
	world_._11 = 0.0f;
	world_._12 = 0.0f;
	world_._13 = 0.0f;
	world_._14 = 0.0f;

	world_._21 = 0.0f;
	world_._22 = 0.0f;
	world_._23 = 0.0f;
	world_._24 = 0.0f;

	world_._31 = 0.0f;
	world_._32 = 0.0f;
	world_._33 = 0.0f;
	world_._34 = 0.0f;

	world_._41 = 0.0f;
	world_._42 = 0.0f;
	world_._43 = 0.0f;
	world_._44 = 0.0f;
}
VertexPosNormTexInstanced::~VertexPosNormTexInstanced()
{
	//Do nothing.
}