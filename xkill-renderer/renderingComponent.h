#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

/*! \defgroup xkill-renderer XKILL - Renderer
	Rendering Component of XKILL. */

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

//! Rendering Component of XKILL.
/*!
Main rendering component of XKILL utilizing Deferred Rendering with 2 G-buffers:
* Albedo
* Normals
\ingroup xkill-renderer
*/
class DLL RenderingComponent : public D3DInterface
{
public:
	//! Initializes RenderingComponent to default values. init()-method need be called in order for RenderingComponent to get proper values.
	/*!
	\param windowHandle Handle to WINAPI-window to which it will render.
	\param screenWidth Width of backbuffer, depthbuffer and g-buffers.
	\param screenHeight Height of backbuffer, depthbuffer and g-buffers.
	\param texAliasing Aliasing-count of backbuffer, depthbuffer and g-buffers.
	*/
	RenderingComponent(
		HWND windowHandle, 
		unsigned int screenWidth, 
		unsigned int screenHeight, 
		unsigned int texAliasing);
	//! Releases all memory and returns to default state.
	~RenderingComponent();

	//! Initializes RenderingComponent's members and prepares render.
	/*!	\return First encountered error.
		\sa initDeviceAndSwapChain
		\sa initDepthBuffer
		\sa initBackBuffer
		\sa initGBuffers
		\sa initViewport
		\sa initRSDefault
		\sa initSSDefault
		\sa initFXManagement
		\sa initDebug
		\sa initConstantBuffers
		\sa initVertexBuffer
	*/
	HRESULT init();
	//! Resets RenderingComponent to default state.
	void reset();
	//! Main render-method of RenderingComponent.
	/*!
	\param view View-matrix from camera.
	\param projection Projection-matrix from camera.
	\sa renderToGBuffer
	\sa renderToBackBuffer
	*/
	void render(MatF4 view, MatF4 projection);
private:
	//! Renders to g-buffers, storing albedo and normals till later.
	/*!
	\param view View-matrix from camera.
	\param projection Projection-matrix from camera.
	*/
	void renderToGBuffer(MatF4 view, MatF4 projection);
	//! Samples from g-buffers and creates a final image in in backbuffer.
	/*!
	\sa rtvBackBuffer
	*/
	void renderToBackBuffer();

	//! Translates the initiated feature-level to string which may be presented in window.
	/*!
	\return The feature-level if known or "Default" otherwize.
	\param featureLevel The initiated feature-level.
	*/
	LPCWSTR featureLevelToString(const D3D_FEATURE_LEVEL featureLevel);
	
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
	//! Creates depth-buffer texture with it's corresponding depth stencil view.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initDepthBuffer();
	//! Gets texture from swapchain and creates corresponding render target view.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initBackBuffer();
	//! Creates GBuffer-objects for each desired g-buffer.
	/*!
	\return Any error encountered during initialization.
	\sa GBuffer
	*/
	HRESULT initGBuffers();
	//! Creates a single viewport onto which the scene will render.
	void initViewport();
	//! Creates rasterizer-state.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initRSDefault();
	//! Creates a single samplerstate in order to sample textures in shaders.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initSSDefault();
	//! Initializes FXManagement-object which will maintain shaders and input-layouts throughout application.
	/*!
	\return Any error encountered during initialization.
	\sa FXManagement
	*/
	HRESULT initFXManagement();
	//! Creates D3DDebug-object which is used for detecting live COM-objects at end of application.
	/*! Warning: D3DDebug recognizes it's own members as live COM-objects, thusly reporting 'false' live objects.
	\return Any error encountered during initialization.
	\sa D3DDebug
	*/
	HRESULT initDebug();
	//! Creates a lone constant buffer that is used to send final matrix to shader.
	/*!
	\return Any error encountered during initialization.
	\sa CBPerFrame
	*/
	HRESULT initConstantBuffers();
	//! Creates a mockup vertexbuffer that loads it's vertices from a basic .obj-loader using bth.obj.
	/*!
	\return Any error encountered during initialization.
	\sa ObjLoaderBasic
	*/
	HRESULT initVertexBuffer();

	/*desc*/
	HWND windowHandle;			//!< WINAPI-handle to window.
	unsigned int screenWidth;	//!< Width of screen
	unsigned int screenHeight;	//!< Height of screen
	unsigned int aliasingCount;	//!< Number of samples used in Anti Aliasing

	FXManagement*	fxManagement;						//!< Maintaining shaders and input-layouts
	GBuffer*		gBuffers[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering 
	D3DDebug*		d3dDebug;							//!< Used for detecting live COM-objects

	ID3D11Device*			device;	//!< DirectX device pointer
	ID3D11DeviceContext*	devcon;	//!< DirectX device context pointer

	IDXGISwapChain*			swapChain;		//!< DirectX swap chain
	ID3D11RenderTargetView*	rtvBackBuffer;	//!< Used to render to texBackBuffer
	ID3D11DepthStencilView*	dsvDepthBuffer;	//!< Used to render to texDepthBuffer
	ID3D11RasterizerState*	rsDefault;		//!< Defines settings for the rasterizer
	ID3D11SamplerState*		ssDefault;		//!< Used to sample from texture in shader

	ID3D11Texture2D* texBackBuffer;		//!< Contains the final image
	ID3D11Texture2D* texDepthBuffer;	//!< Saves the depth of each rendered pixel

	ID3D11Buffer* cbPerFrame;	//!< Buffer sending final matrix to shader

	//temp
	ID3D11Buffer* vertexBuffer;		//!< Mock buffer sending vertices to shader
	std::vector<Vertex>* vertices;	//!< Mock vertices
	ObjLoaderBasic* objLoader;		//!< Basic obj-loader used to debug renderer. 
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H