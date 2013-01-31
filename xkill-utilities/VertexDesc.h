#ifndef XKILL_IO_VERTEXDESC_H
#define XKILL_IO_VERTEXDESC_H

#include "Math.h"

static const unsigned int NUM_BONE_INDICES = 4;
struct DLL_U VertexDesc
{
	VertexDesc();
	~VertexDesc();

	Float3 position_;
	Float3 normal_;
	Float2 textureCoordinates_;
	Float4 tangent_;
	
	unsigned int boneIndices_[NUM_BONE_INDICES];
	Float3 weights_;

	Float3 color_;
};

#endif //XKILL_IO_VERTEXDESC_H