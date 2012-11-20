#ifndef XKILL_RENDERER_GBUFFER_H
#define XKILL_RENDERER_GBUFFER_H

#include <d3d11.h>

#include <d3dInterface.h>

class GBuffer : public D3DInterface
{
public:
	GBuffer(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat);
	~GBuffer();

	void cleanUp();

	HRESULT init(ID3D11Device* device);

	ID3D11ShaderResourceView*	getSRV();
	ID3D11RenderTargetView*		getRTV();
private:
	HRESULT initTex(ID3D11Device* device);
	HRESULT initSRV(ID3D11Device* device);
	HRESULT initRTV(ID3D11Device* device);

	unsigned int	texWidth;
	unsigned int	texHeight;
	unsigned int	texAliasing;
	DXGI_FORMAT		texFormat;

	ID3D11Texture2D*			tex;
	ID3D11ShaderResourceView*	srv;
	ID3D11RenderTargetView*		rtv;
};

#endif