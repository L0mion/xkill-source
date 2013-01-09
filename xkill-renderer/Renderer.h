#ifndef XKILL_RENDERER_RENDERER_H
#define XKILL_RENDERER_RENDERER_H

typedef long HRESULT;
typedef struct HWND__* HWND;

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMFLOAT3;
};

struct Attribute_Render;
struct Attribute_DebugShape;
struct Attribute_Camera;
struct Attribute_Spatial;
struct Attribute_Position;
struct ViewportData;

class Winfo;
class TexDesc;
class IB;
class MeshMaterial;
class ManagementD3D;
class ManagementFX;
class ManagementCB;
class ManagementLight;
class ManagementViewport;
class ManagementModel;
class ManagementTex;
class ManagementSS;
class ManagementRS;
class ManagementGBuffer;
class ManagementDebug;
class ManagementMath;

#include <vector>

//temp
class M3DLoader;
class AnimatedMesh;



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
	HRESULT	resize(
		unsigned int screenWidth, 
		unsigned int screenHeight);	//!< Resizes all management objects that are affected by a change in screen resolution.
	HRESULT	init();					//!< Initializes members and prepares render.
	void	update();
	void	render();	//!< Renders a frame.
	void	loadTextures(TexDesc* texdesc); //!< Forwards information related to what textures Renderer is to load to Renderer-object.
protected:
private:
	void initAttributes();				//!< Retrieves pointers to data vectors which will be used during execution.
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

	void renderViewportToGBuffer(
		ViewportData& vpData);	//!< Renders to g-buffer.
	void renderViewportToBackBuffer(ViewportData& vpData);			//!< Renders to backbuffer.
	void renderAttribute(
		Attribute_Render* renderAt, 
		DirectX::XMFLOAT4X4 viewMatrix, 
		DirectX::XMFLOAT4X4 projectionMatrix);	//!< Renders an attribute.
	void renderSubset(
		IB* ib, 
		MeshMaterial& material);				//!< Renders a subset.
	void renderDebugShape(
		Attribute_DebugShape*	debugShapeAt, 
		unsigned int			shapeIndex,
		DirectX::XMFLOAT4X4		viewMatrix, 
		DirectX::XMFLOAT4X4		projectionMatrix); //!< Renders a debug shape, such as a bounding sphere.

	//temp
	void renderBackBufferClean();	//refactor me
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
	ManagementGBuffer*	managementGBuffer_;		//!< Maintains the G-Buffers of application.
	ManagementDebug*	managementDebug_;		//!< Used for detecting live COM-objects.
	ManagementMath*		managementMath_;		//!< Loads dx-math vectors into generic-type vectors and maintains other math-related functions.

	std::vector<Attribute_Spatial>*		attributesSpatial_;		//!< Holds spatial data. Is fetched only once.
	std::vector<Attribute_Position>*	attributesPosition_;	//!< Holds positional data. Is fetched only once.
	std::vector<Attribute_Render>*		attributesRender_;		//!< Holds objects supposed to be rendered. Is fetched only once.
	std::vector<Attribute_DebugShape>*	attributesDebugShape_;	//!< Holds debug shapes.
	std::vector<int>*					attributesRenderOwner_;	//!< Holds owners of render-attributes.
	std::vector<int>*					attributesCameraOwner_;
	std::vector<Attribute_Camera>*		attributesCamera_;		//!< Holds cameras being rendered to g-buffers. Is fetched only once.

	//temp
	M3DLoader*		m3dLoader_;
	AnimatedMesh*	animatedMesh_;
};			
			
#endif //XKILL_RENDERER_RENDERER_H