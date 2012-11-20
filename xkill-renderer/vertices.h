#ifndef XKILL_RENDERER_VERTEX_H
#define XKILL_RENDERER_VERTEX_H

#include "mathBasic.h"

struct Vertex
{
	VecF3 position;
	VecF3 normal;
	VecF2 texcoord;

	Vertex()
	{
		this->position	= VecF3(0.0f, 0.0f, 0.0f);
		this->normal	= VecF3(0.0f, 0.0f, 0.0f);
		this->texcoord	= VecF2(0.0f, 0.0f);
	}
	Vertex(VecF3 position, VecF3 normal, VecF2 texcoord)
	{
		this->position = position;
		this->normal = normal;
		this->texcoord = texcoord;
	}
};

#endif //XKILL_RENDERER_VERTEX_H