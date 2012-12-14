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

#if defined (DEBUG) || (DEBUG_)
//#include <vld.h>
#endif //DEBUG || DEBUG_

#define TILE_SIZE 16

class ManagementD3D;
class ManagementFX;
class CBManagement;
class ManagementTex;
class ManagementViewport;
class ManagementSS;
class ManagementRS;
class ManagementLight;
class ManagementModel;

class GBuffer;
class D3DDebug;
class Event_WindowResize;

class M3DLoader;
class AnimatedMesh;

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4X4;
};
struct RenderAttribute;
struct CameraAttribute;
struct SpatialAttribute;
struct PositionAttribute;

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
	//! Resets RenderingComponent to default state.
	void reset();
	
	//! Resizes all management objects that are affected by a change in screen resolution.
	/*!
	\param screenWidth The new screen width.
	\param screenHeight the new screen height.
	\return Any error encountered.
	*/
	HRESULT resize(unsigned int screenWidth, unsigned int screenHeight);

	//! Runs a frame for RenderingComponent.
	void onUpdate(float delta);
	//! Receives events for RenderingComponent.
	void onEvent(Event* e);

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
private:
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
	
	HRESULT initModelManagement();

	HRESULT initTexManagement();
	
	HRESULT initViewport();
	//! Creates a SSManaegement object that will maintain sampler states.
	/*!
	\return Any error encountered during initialization.
	\sa SSManagement
	*/
	HRESULT initSSManagement();
	//! Creates a RSManaegement object that will maintain rasterizer states.
	/*!
	\return Any error encountered during initialization.
	\sa RSManagement
	*/
	HRESULT initRSManagement();
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

	//! Sets which viewport to draw to.
	/*!
	\param index The index of the viewport to draw to. 
	*/
	void setViewport(unsigned int index);
	//! Clears the GBuffers with a single color. 
	void clearGBuffers();
	//! Sets the gbuffers as render targets.
	void renderGBufferSetRenderTargets();
	//! Clears all render targets, buffers, shaders etc after render to gbuffers.
	void renderGBufferClean();
	//! Renders to g-buffers, storing albedo and normals till later.
	/*!
	\param view View-matrix from camera.
	\param projection Projection-matrix from camera.
	*/
	void renderViewportToGBuffer(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	//! Samples from g-buffers and creates a final image using DirectCompute.
	/*!
	\sa uavBackBuffer
	*/
	void renderViewportToBackBuffer();
	//! Clears all render targets, buffers, shaders etc after render to back buffer.
	void renderBackBufferClean();

	//! Calculates an objects world matrix.
	/*!
	\return The world matrix
	\param spatialAttribute Contains scale and rotation to be used in calculation.
	\param postionAttribute Contains position to be used in calculation.
	*/
	DirectX::XMFLOAT4X4 calculateWorldMatrix(SpatialAttribute* spatialAttribute,
											 PositionAttribute* positionAttribute);

	//! Calculates a final matrix that is used to transform an object from local space to homogeneous clip space.
	/*!
	\return The calculated matrix
	\param worldMatrix An object's world matrix.
	\param viewMatrix The camera's view matrix.
	\param projectionMatrix The camera's projection matrix.
	*/
	DirectX::XMFLOAT4X4 calculateFinalMatrix(DirectX::XMFLOAT4X4 worldMatrix,
											 DirectX::XMFLOAT4X4 viewMatrix,
											 DirectX::XMFLOAT4X4 projectionMatrix);
	//! Calculates the inverse of a matrix.
	/*!
	\return The inverse of the input matrix.
	\param matrix The matrix to invert.
	*/
	DirectX::XMFLOAT4X4 calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix);
	
	/*desc*/
	HWND windowHandle_;				//!< WINAPI-handle to window.
	unsigned int screenWidth_;		//!< Width of screen.
	unsigned int screenHeight_;		//!< Height of screen.
	unsigned int numViewports_;		//!< Number of viewports that will be used.
	unsigned int csDispatchX_;		//!< Number of threads along the x-axis that will be dispatched by the compute shader.
	unsigned int csDispatchY_;		//!< Number of threads along the y-axis that will be dispatched by the compute shader.
	
	ManagementD3D*		d3dManagement_;
	ManagementFX*		fxManagement_;						//!< Maintaining shaders and input-layouts.
	CBManagement*		cbManagement_;						//!< Maintaining constant buffers.
	ManagementLight*	lightManagement_;					//!< Maintaining lights.
	ManagementViewport* viewportManagement_;				//!< Maintaining viewports.

	ManagementModel*	modelManagement_;
	ManagementTex*		texManagement_;
	
	ManagementSS*		ssManagement_;						//!< Maintaining sampler states.
	ManagementRS*		rsManagement_;						//!< Maintaining rasterizer states.
	
	D3DDebug*			d3dDebug_;							//!< Used for detecting live COM-objects.
	GBuffer*			gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.

	std::vector<RenderAttribute>* renderAttributes_;
	std::vector<CameraAttribute>* cameraAttributes_;

	//! Methods that will be called on events. 
	void event_WindowResize(Event_WindowResize* e);
	void event_PostDescTex(Event_PostDescTex* e);
	
	//TEMP
	M3DLoader* m3dLoader_;
	AnimatedMesh* animatedMesh_;
	void initAnimations();
	void renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H