#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

#include <stdio.h>
#include <d3d11.h>
#include <vector>

#include "dllRenderer.h"
#include "gBufferID.h"
#include "d3dInterface.h"
#include "mathBasic.h"
#include "vertices.h"

class FXManagement;
class GBuffer;
class D3DDebug;
class ObjLoaderBasic;

class DLL RenderingComponent : public D3DInterface
{
public:
	RenderingComponent(
		HWND windowHandle, 
		unsigned int screenWidth, 
		unsigned int screenHeight, 
		unsigned int texAliasing);
	~RenderingComponent();

	HRESULT init();
	void cleanUp();
	void render(MatF4 view, MatF4 projection);
private:
	void renderToGBuffer(MatF4 view, MatF4 projection);
	void renderToBackBuffer();

	LPCWSTR featureLevelToString(const D3D_FEATURE_LEVEL featureLevel);
	HRESULT initDeviceAndSwapChain();
	HRESULT createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc);
	HRESULT initDepthBuffer();
	HRESULT initBackBuffer();
	HRESULT initGBuffers();
	void initViewport();
	HRESULT initRSDefault();
	HRESULT initSSDefault();
	HRESULT initFXManagement();
	HRESULT initDebug();
	HRESULT initConstantBuffers();
	HRESULT initVertexBuffer();

	/*desc*/
	HWND windowHandle;
	unsigned int screenWidth;
	unsigned int screenHeight;
	unsigned int aliasingCount;

	FXManagement*	fxManagement;
	GBuffer*		gBuffers[GBUFFERID_NUM_BUFFERS];
	D3DDebug*		d3dDebug;

	ID3D11Device*			device;
	ID3D11DeviceContext*	devcon;

	IDXGISwapChain*			swapChain;
	ID3D11RenderTargetView*	rtvBackBuffer;
	ID3D11DepthStencilView*	dsvDepthBuffer;
	ID3D11RasterizerState*	rsDefault;
	ID3D11SamplerState*		ssDefault;

	ID3D11Texture2D* texBackBuffer;
	ID3D11Texture2D* texDepthBuffer;

	ID3D11Buffer* cbPerFrame;

	//temp
	ID3D11Buffer* vertexBuffer;
	std::vector<Vertex>* vertices;
	ObjLoaderBasic* objLoader;
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H