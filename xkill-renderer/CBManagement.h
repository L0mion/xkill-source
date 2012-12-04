#ifndef XKILL_RENDERER_CBMANAGEMENT_H
#define XKILL_RENDERER_CBMANAGEMENT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "d3dInterface.h"

static const unsigned int CB_FRAME_INDEX	= 1;
static const unsigned int CB_INSTANCE_INDEX = 2;

class CBManagement : public D3DInterface
{
public:
	CBManagement();
	~CBManagement();

	void reset();

	void updateCBFrame(ID3D11DeviceContext* devcon,
					   DirectX::XMFLOAT4X4	finalMatrix,
					   DirectX::XMFLOAT4X4	viewMatrix,
					   DirectX::XMFLOAT4X4	viewMatrixInverse,
					   DirectX::XMFLOAT4X4	projectionMatrix,
					   DirectX::XMFLOAT4X4	projectionMatrixInverse,
					   DirectX::XMFLOAT3	eyePosition,
					   unsigned int			numLights);
	void updateCBInstance(ID3D11DeviceContext* devcon,
						  unsigned int screenWidth,
						  unsigned int screenHeight);

	void vsSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	void psSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	void csSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);

	HRESULT init(ID3D11Device* device);
private:

	HRESULT initCBFrame(ID3D11Device* device);
	HRESULT initCBInstance(ID3D11Device* device);

	ID3D11Buffer* cbFrame_;
	ID3D11Buffer* cbInstance_;
};

#endif