#ifndef XKILL_RENDERER_VIEWPORTMANAGEMENT_H
#define XKILL_RENDERER_VIEWPORTMANAGEMENT_H

struct Attribute_SplitScreen;

class Winfo;

#include <vector>
#include <d3d11.h>

#include "d3dInterface.h"

struct SplitScreenViewport
{
	A_Ptr<Attribute_SplitScreen> ptr_splitScreen;
	D3D11_VIEWPORT viewport;
};

//! Class for maintaining viewports.
/*!
\ingroup xkill-renderer
*/
class ManagementViewport : public D3DInterface
{
public:
	ManagementViewport(Winfo* winfo); //!< Sets ViewportManagement to its default state.
	
	~ManagementViewport(); //!< Releases all memory and returns ViewportManagement to default state.
	virtual void reset(); //!< Releases all memory and returns ViewportManagement to default state.
	void setViewport(ID3D11DeviceContext* devcon, unsigned int splitScreenViewportIndex); //!< Set the viewport that the renderer will draw to. 
	
	HRESULT resize();	//!< Resizes the viewports to fit the new screen size.
	HRESULT init();		//!< Initializes ViewportManagement

	unsigned int getViewportWidth() const;	
	unsigned int getViewportHeight() const;	
	unsigned int getNumViewportsX() const;	
	unsigned int getNumViewportsY() const;

	std::vector<SplitScreenViewport>* getSplitScreenViewports();
private:
	HRESULT initViewportSingle(); //!< Creates a single viewport that covers the entire window.
	HRESULT initViewportDouble(); //!< Creates two viewports that splits the window horizontally
	HRESULT initViewportGrid(unsigned int gridSize); //!< Creates a specified amount of viewports in a grid pattern.

	Winfo* winfo_; //!< Shared struct containing data related to screen dimensions.

	unsigned int numViewportsX_;	//!< Number of viewports horisontally.
	unsigned int numViewportsY_;	//!< Number of viewports vertically.
	unsigned int viewportWidth_;	//!< Width of each viewport.
	unsigned int viewportHeight_;	//!< Height of each viewport.
	
	unsigned int borderSize_;	//!< Adds space between viewports.

	std::vector<SplitScreenViewport>* splitScreenViewports_; //!< Current viewports.
};

#endif //XKILL_RENDERER_VIEWPORTMANAGEMENT_H