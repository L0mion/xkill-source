#ifndef XKILL_RENDERER_VIEWPORTMANAGEMENT_H
#define XKILL_RENDERER_VIEWPORTMANAGEMENT_H

#include <vector>
#include <d3d11.h>

#include "d3dInterface.h"

//! Class for maintaining viewports.
/*!
\ingroup xkill-renderer
*/
class ManagementViewport : public D3DInterface
{
public:
	//! Sets ViewportManagement to its default state.
	/*!
	\param numViewports The number of viewports that will be used.
	\param viewportWidth The width of each viewport.
	\param viewportHeight The height of each viewport.
	\param screenWidth The width of the screen in which the viewports reside.
	\param screenHeight The height of the screen in which the viewports reside.
	*/
	ManagementViewport(unsigned int numViewports,
						unsigned int screenWidth,
						unsigned int screenHeight);
	//! Releases all memory and returns ViewportManagement to default state.
	~ManagementViewport();
	//! Releases all memory and returns ViewportManagement to default state.
	virtual void reset();
	//! Set the viewport that the renderer will draw to. 
	void setViewport(ID3D11DeviceContext* devcon, unsigned int index);
	//! Resizes the viewports to fit the new screen size.
	/*!
	\param screenWidth The new screen width.
	\param screenHeight the new screen height.
	\return Any error encountered.
	*/
	HRESULT resize(unsigned int screenWidth, unsigned int screenHeight);
	//! Initializes ViewportManagement.
	HRESULT init();

	unsigned int getViewportWidth() const;
	unsigned int getViewportHeight() const;
	unsigned int getNumViewportsX() const;
	unsigned int getNumViewportsY() const;

	D3D11_VIEWPORT getViewport(unsigned int index) const;

private:
	//! Creates a single viewport that covers the entire window.
	HRESULT initViewportSingle();
	//! Creates two viewports that splits the window horizontally
	HRESULT initViewportDouble();
	//! Creates a specified amount of viewports in a grid pattern.
	/*!
	\param gridSize Total number of viewports in the grid. Must be a perfect square!
	*/
	HRESULT initViewportGrid(unsigned int gridSize);

	unsigned int numViewports_;		//!< Number of viewports that will be used.
	unsigned int numViewportsX_;
	unsigned int numViewportsY_;
	unsigned int viewportWidth_;	//!< Width of each viewport.
	unsigned int viewportHeight_;	//!< Height of each viewport.
	unsigned int screenWidth_;		//!< Width of the screen.
	unsigned int screenHeight_;		//!< Height of the screen.

	unsigned int borderSize_;	//<! Adds space between viewports.

	std::vector<D3D11_VIEWPORT>* viewports_;	//!< Vector containing all the viewport objects.
};

#endif //XKILL_RENDERER_VIEWPORTMANAGEMENT_H