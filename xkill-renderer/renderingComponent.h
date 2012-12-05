#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

/*! \defgroup xkill-renderer xkill-renderer
	Rendering Component of XKILL. */

#include <stdio.h>
#include <d3d11.h>
#include <vector>

#include <xkill-utilities/IObserver.h>

#include "dllRenderer.h"
#include "gBufferID.h"

class FXManagement;
class CBManagement;
class ViewportManagement;
class GBuffer;
class D3DDebug;
class LightManagement;

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4X4;
};
struct RenderAttribute;
struct CameraAttribute;
struct SpatialAttribute;
struct PositionAttribute;

static const unsigned int MULTISAMPLES_GBUFFERS		= 1;
static const unsigned int MULTISAMPLES_BACKBUFFER	= 1;
static const unsigned int MULTISAMPLES_DEPTHBUFFER	= 1;

struct VertexPosNormTex;

//! Rendering Component of XKILL.
/*!
Main rendering component of XKILL utilizing Deferred Rendering with 2 G-buffers:
* Albedo
* Normals
Warning: RenderingComponent may not be created with an anti-aliasing MSAA-count of +0!
\ingroup xkill-renderer
*/
class DLL_R RenderingComponent : public IObserver
{
public:
	//! Initializes RenderingComponent to default values. init()-method need be called in order for RenderingComponent to get proper values.
	/*!
	\param windowHandle	Handle to WINAPI-window to which it will render.
	\param screenWidth	Width of backbuffer, depthbuffer and g-buffers.
	\param screenHeight	Height of backbuffer, depthbuffer and g-buffers.
	\param viewportWidth Width of each viewport.
	\param viewportHeight Height of each viewport. 
	*/
	RenderingComponent(
		HWND windowHandle);
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
	//! Runs a frame for RenderingComponent.
	void onUpdate(float delta);
	//! Receives events for RenderingComponent.
	void onEvent(Event* e);
	

private:
	
	//! Main render-method of RenderingComponent.
	/*!
	\param view View-matrix from camera.
	\param projection Projection-matrix from camera.
	\sa renderToGBuffer
	\sa renderToBackBuffer
	*/
	void render(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);
	//! Renders to g-buffers, storing albedo and normals till later.
	/*!
	\param view View-matrix from camera.
	\param projection Projection-matrix from camera.
	*/
	void renderToGBuffer(DirectX::XMFLOAT4X4 view,
						 DirectX::XMFLOAT4X4 viewInverse,
						 DirectX::XMFLOAT4X4 projection,
						 DirectX::XMFLOAT4X4 projectionInverse,
						 DirectX::XMFLOAT3	eyePosition);
	//! Samples from g-buffers and creates a final image using DirectCompute.
	/*!
	\sa uavBackBuffer
	*/
	void renderToBackBuffer();
	//! Sets which viewport to draw to.
	/*!
	\param index The index of the viewport to draw to. 
	*/
	void setViewport(unsigned int index);
	//! Clears the GBuffers with a single color. 
	void clearGBuffers();

	void gBufferRenderUpdateConstantBuffers(DirectX::XMFLOAT4X4 finalMatrix,
											DirectX::XMFLOAT4X4 viewMatrix,
											DirectX::XMFLOAT4X4 viewInverseMatrix,
											DirectX::XMFLOAT4X4 projectionMatrix,
											DirectX::XMFLOAT4X4 projectionInverseMatrix,
											DirectX::XMFLOAT3	eyePosition);
	void gBufferRenderClean();
	void gBufferRenderSetRenderTargets();
	DirectX::XMFLOAT4X4 calculateFinalMatrix(DirectX::XMFLOAT4X4 viewMatrix,
											 DirectX::XMFLOAT4X4 projectionMatrix,
											 SpatialAttribute spatialAttribute,
											 PositionAttribute positionAttribute,
											 unsigned int attributeIndex);
	DirectX::XMFLOAT4X4 calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix);

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
	//! Gets texture from swapchain and creates corresponding render target view and UAV.
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
	//! Creates a ViewportManagement object that in turn will create the specified amount of viewports.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initViewport();
	
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
	//! Initializes CBManagement-object which will maintain constant buffers.
	/*!
	\return Any error encountered during initialization.
	\sa CBManagement
	*/
	HRESULT initCBManagement();
	//! Initializes LightManagement-object which will maintain lights.
	/*!
	\return Any error encountered during initialization.
	\sa LightManagement
	*/
	HRESULT initLightManagement();
	//! Creates D3DDebug-object which is used for detecting live COM-objects at end of application.
	/*! Warning: D3DDebug recognizes it's own members as live COM-objects, thusly reporting 'false' live objects.
	\return Any error encountered during initialization.
	\sa D3DDebug
	*/
	HRESULT initDebug();

	/*desc*/
	HWND windowHandle_;				//!< WINAPI-handle to window.
	unsigned int screenWidth_;		//!< Width of screen.
	unsigned int screenHeight_;		//!< Height of screen.
	unsigned int numViewports_;		//!< NUmber of viewports that will be used.
	
	FXManagement*		fxManagement_;						//!< Maintaining shaders and input-layouts.
	CBManagement*		cbManagement_;						//!< Maintaining constant buffers.
	LightManagement*	lightManagement_;					//!< Maintaining lights.
	ViewportManagement* viewportManagement_;				//!< Maintaining viewports.
	GBuffer*			gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.
	D3DDebug*			d3dDebug_;							//!< Used for detecting live COM-objects.

	std::vector<RenderAttribute>* renderAttributes_;
	std::vector<CameraAttribute>* cameraAttributes_;

	ID3D11Device*			device_;	//!< DirectX device pointer.
	ID3D11DeviceContext*	devcon_;	//!< DirectX device context pointer.

	IDXGISwapChain*			swapChain_;			//!< DirectX swap chain.
	ID3D11RenderTargetView*	rtvBackBuffer_;		//!< Used to render to texBackBuffer.
	ID3D11DepthStencilView*	dsvDepthBuffer_;	//!< Used to render to texDepthBuffer.
	ID3D11RasterizerState*	rsDefault_;			//!< Defines settings for the rasterizer.
	ID3D11SamplerState*		ssDefault_;			//!< Used to sample from texture in shader.

	ID3D11Texture2D* texBackBuffer_;	//!< Contains the final image.
	ID3D11Texture2D* texDepthBuffer_;	//!< Saves the depth of each rendered pixel.

	//direct compute
	ID3D11UnorderedAccessView* uavBackBuffer_; //!< Used to render to texBackBuffer using DirectCompute.

	//std::vector<
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H