#ifndef XKILL_RENDERER_INSTANCEDDATADESC_H
#define XKILL_RENDERER_INSTANCEDDATADESC_H

#include <DirectXMath.h>

struct VertexInstanced
{
	VertexInstanced();
	~VertexInstanced();

	DirectX::XMFLOAT4X4 world_;
	//int cull;
};

#endif //XKILL_RENDERER_INSTANCEDDATADESC_H