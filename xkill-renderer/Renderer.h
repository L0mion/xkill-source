#ifndef XKILL_RENDERER_RENDERER_H
#define XKILL_RENDERER_RENDERER_H

typedef long HRESULT;
typedef struct HWND__* HWND;

template<typename T>
class DataStreamBuffer;
struct VertexInstanced;
typedef DataStreamBuffer<VertexInstanced> InstancedData;

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
	struct XMFLOAT4X4;
};

struct ViewportData;

class ManagementD3D;
class ManagementFX;
class ManagementCB;
class ManagementLight;
class ManagementViewport;
class ManagementModel;
class ManagementTex;
class ManagementSS;
class ManagementRS;
class ManagementBuffer;
class ManagementDebug;
class ManagementMath;
class ManagementInstance;
class ManagementSprites;

class Winfo;
class TexDesc;
class MeshMaterial;
class SubsetD3D;

#include "ShadingDesc.h"
#include "ShadowMapping.h"

//#define VISUALLEAKDETECTOR
#if (defined(DEBUG) || defined(_DEBUG)) && defined(VISUALLEAKDETECTOR)
	#include <vld.h>
#endif

//temp
class M3DLoader;
class AnimatedMesh;
struct ID3D11Buffer; //Bullet Physics lines

//! Module responsible for managing data related to rendering and doing the actual rendering of XKILL.
/*! Rendering module of XKILL utilizing Deferred Rendering with DirectCompute.
\ingroup xkill-renderer
*/
class Renderer
{
public:
	Renderer(HWND windowHandle);	//!< Initializes RenderingComponent to default values. init()-method need be called in order for RenderingComponent to get proper values.
	~Renderer();					//!< Releases all memory and returns to default state.
	
	void	reset();				//!< Resets RenderingComponent to default state.
	void	unloadModels();			//!< Unloads all models in ManagementModel
	HRESULT	resize(
		unsigned int screenWidth, 
		unsigned int screenHeight);	//!< Resizes all management objects that are affected by a change in screen resolution.
	HRESULT	init();					//!< Initializes members and prepares render.
	void	update();
	void	render();	//!< Renders a frame.
	void	loadTextures(TexDesc* texdesc); //!< Forwards information related to what textures Renderer is to load to Renderer-object.
protected:
private:
	void initWinfo();					//!< Sends resolution event to find out current resolution, and stores this info in a Winfo-type object. This object is then shared as a pointer amongst Renderer's members.
	HRESULT initManagementD3D();		//!< Initializes ManagementD3D-object which will maintain core DirectX objects, e.g. device and device context.
	HRESULT initManagementFX();			//!< Initializes ManagementFX-object which will maintain shaders and input-layouts throughout application.
	HRESULT initManagementCB();			//!< Initializes ManagementCB-object which will maintain constant buffers.
	HRESULT initManagementLight();		//!< Initializes ManagementLight-object which will maintain lights.
	HRESULT initManagementModel();		//!< Initializes ManegementModel, which maintains the renderer's view of meshes.
	HRESULT initManagementTex();		//!< Initializes ManagementTex, which maintains loaded textures.
	HRESULT initManagementViewport();	//!< Creates a ManagementViewport object that in turn will create the specified amount of viewports.
	HRESULT initManagementSS();			//!< Creates a ManagementSS object that will maintain sampler states.
	HRESULT initManagementRS();			//!< Creates a ManagementRS object that will maintain rasterizer states.
	HRESULT initManagementGBuffer();	//!< Creates a ManagementGBuffer-type object that will maintain the application's g-buffers.
	HRESULT initManagementDebug();		//!< Initializes ManagementDebug, which holds data allowing advanced detection of COM-leaks in D3D.
	void	initManagementMath();		//!< Initializes ManagementMath, which manages math-related functions and loading of dx-vectors into generic-type vectors utilizing SIMD.
	void	initManagementInstance();	//!< Initializes ManagementInstance, which manages all the instances of the various models.
	HRESULT initManagementSprites();	//!< Initializes ManagementSprites, which manages all sprites.

	void renderViewportToGBuffer(
		ViewportData& vpData);											//!< Renders to g-buffer.
	void renderViewportToBackBuffer(ViewportData& vpData);				//!< Renders to backbuffer.
	void renderInstance(unsigned int meshID, InstancedData* instance, bool shadowmap);	//!< Renders an instanced model.
	ShadingDesc deriveShadingDesc(VertexType vertexType, bool shadowmap);
	void setShadingDesc(ShadingDesc shadingDesc);
	void renderSubset(
		SubsetD3D* subset, 
		MaterialDesc& material,
		unsigned int numInstances,
		bool shadowmap);	//!< Renders a subset.

	//Shadows
	DirectX::XMFLOAT4X4	buildShadowMap();
	ShadowMatrices constructShadowMatrices(SceneBounds bounds, Float3 lightDirection);

	//Glow effect
	void downSampleBlur();
	void blurHorizontally();
	void blurVertically();
	void upSampleBlur();

	//SSAO
	void buildSSAOMap(ViewportData& vpData);

	void drawBulletPhysicsDebugLines(
		DirectX::XMFLOAT4X4		viewMatrix, 
		DirectX::XMFLOAT4X4		projectionMatrix); //!<A vertex buffer is recreated when a EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES event is present in the event queue.

	void drawLaser(
		DirectX::XMFLOAT4X4		viewMatrix,
		DirectX::XMFLOAT4X4		projectionMatrix); //!< Draw all lasers using ray attributes

	void renderHudElements(int viewportIndex); //!< Reders hud elements such as the cross hair.
	void renderHudElementCrossHair(int viewportIndex, float scaleModifierX); //!< Renders a cross hair in the middle óf each viewport.
	void drawHudElement(int viewportIndex, unsigned int textureId, DirectX::XMFLOAT4X4 transformationMatrix); //!< Draws a single hud element.

	//temp
	void renderAnimatedMesh(
		DirectX::XMFLOAT4X4 viewMatrix, 
		DirectX::XMFLOAT4X4 projectionMatrix);
	
	HWND	windowHandle_;	//!< Handle to WinAPI-window.
	Winfo*	winfo_;			//!< Holds information related to screen dimensions. Object is shared thruought Renderer's members.

	ManagementD3D*		managementD3D_;			//!< Maintains core D3D-objects.
	ManagementFX*		managementFX_;			//!< Maintaining shaders and input-layouts.
	ManagementCB*		managementCB_;			//!< Maintaining constant buffers.
	ManagementLight*	managementLight_;		//!< Maintaining lights.
	ManagementViewport* managementViewport_;	//!< Maintaining viewports.
	ManagementModel*	managementModel_;		//!< Maintains the rendering view of a mesh, such as vertex- and index-buffers.
	ManagementTex*		managementTex_;			//!< Maintains textures to be used in shaders.
	ManagementSS*		managementSS_;			//!< Maintaining sampler states.
	ManagementRS*		managementRS_;			//!< Maintaining rasterizer states.
	ManagementBuffer*	managementBuffer_;		//!< Maintains the G-Buffers of application.
	ManagementDebug*	managementDebug_;		//!< Used for detecting live COM-objects.
	ManagementMath*		managementMath_;		//!< Loads dx-math vectors into generic-type vectors and maintains other math-related functions.
	ManagementInstance*	managementInstance_;	//!< Maintains all instances of respective model in the game.
	ManagementSprites*  managementSprites_;		//!< Maintaining sprites.

	ID3D11Buffer* debugLinesVertexBuffer_;		//!< Might want to move this into some manager of some sort.
	ID3D11Buffer*			rayBuffer;

	ShadowMatrices shadowMatrices_;

	//temp
	M3DLoader*		m3dLoader_;
	AnimatedMesh*	animatedMesh_;
};			
			
#endif //XKILL_RENDERER_RENDERER_H