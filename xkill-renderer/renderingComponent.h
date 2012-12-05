#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

/*! \defgroup xkill-renderer xkill-renderer
	Rendering Component of XKILL. */

#include <stdio.h>
#include <d3d11.h>
#include <vector>

#include "dllRenderer.h"
#include "gBufferID.h"
//#include "d3dInterface.h"
#include <xkill-utilities/IObserver.h>

class D3DManagement;
class FXManagement;
class CBManagement;
class ViewportManagement;
class SSManagement;
class GBuffer;
class D3DDebug;
class ObjLoaderBasic;
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

//static const unsigned int MULTISAMPLES_GBUFFERS		= 1;
//static const unsigned int MULTISAMPLES_BACKBUFFER	= 1;
//static const unsigned int MULTISAMPLES_DEPTHBUFFER	= 1;

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
	void renderClean();
	void gBufferRenderSetRenderTargets();
	DirectX::XMFLOAT4X4 calculateFinalMatrix(DirectX::XMFLOAT4X4 viewMatrix,
											 DirectX::XMFLOAT4X4 projectionMatrix,
											 SpatialAttribute spatialAttribute,
											 PositionAttribute positionAttribute,
											 unsigned int attributeIndex);
	DirectX::XMFLOAT4X4 calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix);

	//! Initializes D3DManagement-object which will maintain core DirectX objects, e.g. device and device context.
	/*!
	\return Any error encountered during initialization.
	\sa D3DManagement
	*/
	HRESULT initD3DManagement();
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
	//! Creates a ViewportManagement object that in turn will create the specified amount of viewports.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initViewport();
	//! Creates a SSManaegement object that will maintain sampler states.
	/*!
	\return Any error encountered during initialization.
	\sa SSManagement
	*/
	HRESULT initSSManagement();
	//! Creates GBuffer-objects for each desired g-buffer.
	/*!
	\return Any error encountered during initialization.
	\sa GBuffer
	*/
	HRESULT initGBuffers();
	//! Creates D3DDebug-object which is used for detecting live COM-objects at end of application.
	/*! Warning: D3DDebug recognizes it's own members as live COM-objects, thusly reporting 'false' live objects.
	\return Any error encountered during initialization.
	\sa D3DDebug
	*/
	HRESULT initDebug();
	//! Creates a mockup vertexbuffer that loads it's vertices from a basic .obj-loader using bth.obj.
	/*!
	\return Any error encountered during initialization.
	\sa ObjLoaderBasic
	*/
	HRESULT initVertexBuffer();

	/*desc*/
	HWND windowHandle_;				//!< WINAPI-handle to window.
	unsigned int screenWidth_;		//!< Width of screen.
	unsigned int screenHeight_;		//!< Height of screen.
	unsigned int numViewports_;		//!< NUmber of viewports that will be used.
	
	D3DManagement*		d3dManagement_;
	FXManagement*		fxManagement_;						//!< Maintaining shaders and input-layouts.
	CBManagement*		cbManagement_;						//!< Maintaining constant buffers.
	LightManagement*	lightManagement_;					//!< Maintaining lights.
	ViewportManagement* viewportManagement_;				//!< Maintaining viewports.
	SSManagement*		ssManagement_;						//!< Maintaining sampler states.
	D3DDebug*			d3dDebug_;							//!< Used for detecting live COM-objects.
	GBuffer*			gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.

	std::vector<RenderAttribute>* renderAttributes_;
	std::vector<CameraAttribute>* cameraAttributes_;
	

	//temp
	ID3D11Buffer*			vertexBuffer_;		//!< Mock buffer sending vertices to shader.
	std::vector<VertexPosNormTex>*	vertices_;	//!< Mock vertices.
	ObjLoaderBasic*			objLoader_;			//!< Basic obj-loader used to debug renderer. 
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H