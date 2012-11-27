#include "ViewportManagement.h"
#include "renderingUtilities.h"

ViewportManagement::ViewportManagement(	unsigned int numViewports,
										unsigned int screenWidth,
										unsigned int screenHeight)
{
	numViewports_	= numViewports;
	screenWidth_	= screenWidth;
	screenHeight_	= screenHeight;

	viewportWidth_	= 0;
	viewportHeight_ = 0;

	border_ = 4.0f;
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
	
	viewportWidth_	= screenWidth_;
	viewportHeight_ = screenHeight_;

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(screenHeight_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	viewports->push_back(viewport);

	return hr;
}
HRESULT ViewportManagement::initViewportDouble()
{
	HRESULT hr = S_OK;

	viewportWidth_	= screenWidth_;
	viewportHeight_ = screenHeight_/2;

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(viewportWidth_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_)-border_;
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	viewports->push_back(viewport);

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= static_cast<FLOAT>(viewportHeight_)+border_;

	viewports->push_back(viewport);

	return hr;
}
HRESULT ViewportManagement::initViewportGrid(unsigned int gridSize)
{

	HRESULT hr = S_OK;
	unsigned int width = static_cast<unsigned int>(sqrt(gridSize));
	
	viewportWidth_	= screenWidth_/width;
	viewportHeight_ = screenHeight_/width;

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

			if(column != 0)
				viewport.TopLeftY += border_;
			if(row != 0)
				viewport.TopLeftX += border_;

			viewports->push_back(viewport);
		}
	}

	return hr;
}

unsigned int ViewportManagement::getViewportWidth() const
{
	return viewportWidth_;
}

unsigned int ViewportManagement::getViewportHeight() const
{
	return viewportHeight_;
}