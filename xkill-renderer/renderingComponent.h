/** \defgroup xkill-renderer xkill-renderer
 *
 * \brief Rendering package of XKILL
 */

/**
 * \class RenderingComponent
 * \ingroup xkill-renderer
 * \brief Main rendering class
 * This class renders stuff
 * \note This is a note.
 * \author (last to touch it) $Author: David Grelsson, Eric Nilsson $
 */

#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

#include <stdio.h>
#include <d3d11.h>

#include "dllRenderer.h"
#include "fxManagement.h"

class DLL RenderingComponent
{
public:
	/// Short description: Mock function
	RenderingComponent(HWND windowHandle, UINT screenWidth, UINT screenHeight, UINT aliasingCount);
	~RenderingComponent();

	HRESULT init();
	void render();
protected:
private:

	LPCWSTR featureLevelToString(const D3D_FEATURE_LEVEL featureLevel);
	HRESULT initDeviceAndSwapChain();
	HRESULT createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc);
	HRESULT initDepthBuffer();
	HRESULT initRenderTargets();
	HRESULT initBackBuffer();
	void initViewport();
	HRESULT initRasterizerState();
	HRESULT initFXManagement();

	HWND windowHandle;
	UINT screenWidth;
	UINT screenHeight;
	UINT aliasingCount;

	ID3D11Device*			device;
	ID3D11DeviceContext*	devcon;

	FXManagement* fxManagement;

	IDXGISwapChain*			swapChain;
	ID3D11RenderTargetView*	renderTargets[2];
	ID3D11RenderTargetView*	backBuffer;
	ID3D11DepthStencilView*	zBuffer;
	ID3D11RasterizerState*	rasterizerState;
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H