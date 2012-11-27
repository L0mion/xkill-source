#include "ViewportManagement.h"
#include "renderingUtilities.h"

ViewportManagement::ViewportManagement(	unsigned int numViewports,
										unsigned int viewportWidth,
										unsigned int viewportHeight,
										unsigned int screenWidth,
										unsigned int screenHeight)
{
	numViewports_	= numViewports;
	viewportWidth_	= viewportWidth;
	viewportHeight_ = viewportHeight;
	screenWidth_	= screenWidth;
	screenHeight_	= screenHeight;
}

ViewportManagement::~ViewportManagement()
{
	SAFE_DELETE(viewports);
}

void ViewportManagement::reset()
{
	SAFE_DELETE(viewports);
}

void ViewportManagement::setViewport(ID3D11DeviceContext* devcon, unsigned int index)
{
	devcon->RSSetViewports(1, &viewports->at(index));
}

HRESULT ViewportManagement::init()
{
	viewports = new std::vector<D3D11_VIEWPORT>();

	unsigned int gridSizes[] = {1, 2, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
	unsigned int gridSize = 0;
	unsigned int index = 0;
	HRESULT hr = E_FAIL;
	while(FAILED(hr) && index<11)
	{
		if(numViewports_ <= gridSizes[index])
		{
			gridSize = gridSizes[index];
			hr = S_OK;
		}
		index++;
	}
	
	if(SUCCEEDED(hr))
	{
		if(gridSize == 1)
			hr = initViewportSingle();
		else if(gridSize == 2)
			hr = initViewportDouble();
		else
			hr = initViewportGrid(gridSize);
	}

	return hr;
}

HRESULT ViewportManagement::initViewportSingle()
{
	HRESULT hr = S_OK;
	if(viewportWidth_ > screenWidth_ || viewportHeight_ > screenHeight_)
	{
		hr = E_FAIL;
		ERROR_MSG(L"RenderingComponent::initViewportSingle() failed! Viewport size exceeds screen size!");
	}
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(viewportWidth_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	viewports->push_back(viewport);

	return hr;
}
HRESULT ViewportManagement::initViewportDouble()
{
	HRESULT hr = S_OK;
	if(viewportWidth_ > screenWidth_ || (viewportHeight_*2) > screenHeight_)
	{
		hr = E_FAIL;
		ERROR_MSG(L"RenderingComponent::initViewportDouble() failed! Viewport size exceeds screen size!");
	}
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(viewportWidth_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	viewports->push_back(viewport);

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= static_cast<FLOAT>(viewportHeight_);

	viewports->push_back(viewport);

	return hr;
}
HRESULT ViewportManagement::initViewportGrid(unsigned int gridSize)
{

	HRESULT hr = S_OK;
	unsigned int width = static_cast<unsigned int>(sqrt(gridSize));
	if((viewportWidth_*width) > screenWidth_ || (viewportHeight_*width) > screenHeight_)
	{
		hr = E_FAIL;
		ERROR_MSG(L"RenderingComponent::initViewportDouble() failed! Viewport size exceeds screen size!");
	}
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(viewportWidth_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	for(unsigned int row = 0; row<width; row++)
	{
		for(unsigned int column=0; column<width; column++)
		{
			viewport.TopLeftX = static_cast<FLOAT>(row*viewportWidth_);
			viewport.TopLeftY = static_cast<FLOAT>(column*viewportHeight_);

			viewports->push_back(viewport);
		}
	}

	return hr;
}