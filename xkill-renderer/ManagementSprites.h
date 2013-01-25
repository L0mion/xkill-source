#ifndef XKILL_RENDERER_MANAGEMENTSPRITES_H
#define XKILL_RENDERER_MANAGEMENTSPRITES_H

#include <d3d11.h>
#include <xkill-utilities/MeshVertices.h>
#include "d3dInterface.h"

static const unsigned int NUM_SPRITE_VERTICES	= 4;
static const unsigned int NUM_SPRITE_INDICES	= 6;

class ManagementSprites : public D3DInterface
{
public:
	ManagementSprites();
	virtual ~ManagementSprites();

	virtual void reset();

	HRESULT init(ID3D11Device* device);

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
private:

	void initVertices();
	void initIndices();
	HRESULT initVertexBuffer(ID3D11Device* device);
	HRESULT initIndexBuffer(ID3D11Device* device);

	VertexPosNormTex vertices_[NUM_SPRITE_VERTICES];
	DWORD indices_[NUM_SPRITE_INDICES];

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
};

#endif // XKILL_RENDERER_MANAGEMENTSPRITES_H