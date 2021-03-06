#ifndef XKILL_RENDERER_D3DMANAGEMENT_H
#define XKILL_RENDERER_D3DMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4X4;
}

struct VertexPosNormTex;

class Winfo;
class Buffer_SrvDsv;
class Buffer_SrvRtvUav;

static const unsigned int GBUFFER_SHADER_REGISTER_DEPTH = 6;

static const FLOAT CLEAR_COLOR_BACKBUFFER_BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT CLEAR_COLOR_BACKBUFFER_WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//! Class for maintaining DirectX core objects.
/*!
\ingroup xkill-renderer
*/
class ManagementD3D : public D3DInterface
{
public:
	//! Sets D3DManagement to its default state.
	/*!
	\param windowHandle	A handle to a WIN-API window.
	\param winfo		Pointer to Renderer's window-info attribute.
	*/
	ManagementD3D(HWND windowHandle, Winfo* winfo);
	//!Releases all memory and resets D3DManagement to its default state.
	virtual ~ManagementD3D();
	//!Releases all memory and resets D3DManagement to its default state.
	virtual void reset();
	
	//! Resizes textures and back buffers to fit the new screen size.
	/*!
	\return Any error encountered.
	*/
	HRESULT resize();

	//! Set the variable uavBackBuffer to the compute shader stage.
	void setUAVBackBufferCS();
	
	void unsetUAVBackBufferCS();

	void setDepthBufferSRV(unsigned int shaderRegister);
	void unsetDepthBufferSRV(unsigned int shaderRegister);

	//! Sets the variable rtvBackBuffer as a render target.
	void setRenderTargetViewBackBuffer();
	//! Removes rtvBackBuffer as render target.
	void unsetRenderTargetViewBackBuffer();

	//! Presents the back buffer.
	void present();
	//! Clears the depth buffer in preparation for rendering. 
	void clearDepthBuffer();

	void clearBackBuffer();

	HRESULT init();

	ID3D11Device*				getDevice()				const;
	ID3D11DeviceContext*		getDeviceContext()		const;
	ID3D11DepthStencilView*		getDepthBuffer()		const;
	ID3D11Texture2D*			getDepthBufferTexture() const;
	ID3D11ShaderResourceView*	getDepthBufferSRV()		const;
private:
	//! Initializes struct describing swapchain using values passed in constructor.
	/*!
	\return Any error encountered during initialization.
	\sa createDeviceAndSwapChain
	*/
	HRESULT initDeviceAndSwapChain();
	//! Creates device and swap chain using correct feature-level based on hardware.
	/*!
	\param swapChainDesc Description of d3dswapchain.
	\return Any error encountered during initialization.
	*/
	HRESULT createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc);
	//! Gets texture from swapchain and creates corresponding render target view and UAV.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initBackBuffer();
	//! Creates depth-buffer texture with it's corresponding depth stencil view.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initDepthBuffer();
	//! Translates the initiated feature-level to string which may be presented in window.
	/*!
	\return The feature-level if known or "Default" otherwize.
	\param featureLevel The initiated feature-level.
	*/
	LPCWSTR featureLevelToString(const D3D_FEATURE_LEVEL featureLevel);

	HWND						windowHandle_;						//!< WINAPI-handle to window.
	Winfo*						winfo_;

	ID3D11Device*				device_;							//!< DirectX device pointer.
	ID3D11DeviceContext*		devcon_;							//!< DirectX device context pointer.
	IDXGISwapChain*				swapChain_;							//!< DirectX swap chain.
	ID3D11RenderTargetView*		rtvBackBuffer_;						//!< Used to render to texBackBuffer.
	ID3D11UnorderedAccessView*	uavBackBuffer_;						//!< Used to render to texBackBuffer using DirectCompute.

	Buffer_SrvDsv* depthBuffer_;

	ID3D11Texture2D*			texBackBuffer_;						//!< Contains the final image.6
};

#endif //XKILL_RENDERER_D3DMANAGEMENT_H