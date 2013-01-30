#include "VertexDesc.h"

VertexDesc::VertexDesc()
{
	position_ = Float3(0.0f, 0.0f, 0.0f);
	normal_ = Float3(0.0f, 0.0f, 0.0f);
	textureCoordinates_ = Float2(0.0f, 0.0f);
	tangent_ = Float4(0.0f, 0.0f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < NUM_BONE_INDICES; i++)
	{
		boneIndices_[i] = 0;
	}
	weights_ = Float3(0.0f, 0.0f, 0.0f);
}
VertexDesc::~VertexDesc()
{
	//Do nothing.
}