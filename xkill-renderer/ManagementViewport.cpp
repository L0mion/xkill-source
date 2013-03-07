#include <xkill-utilities/Util.h>

#include "Winfo.h"
#include "ManagementViewport.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementViewport::ManagementViewport(Winfo* winfo)
{
	winfo_ = winfo;

	viewportWidth_	= 0;
	viewportHeight_ = 0;
	numViewportsX_	= 1;
	numViewportsY_	= 1;

	borderSize_ = 2;

	splitScreenViewports_ = nullptr;

	ATTRIBUTES_INIT_ALL;
}
ManagementViewport::~ManagementViewport()
{
	SAFE_DELETE(splitScreenViewports_);
}

void ManagementViewport::reset()
{
	SAFE_DELETE(splitScreenViewports_);
}

void ManagementViewport::setViewport(ID3D11DeviceContext* devcon, unsigned int splitScreenViewportIndex)
{
	devcon->RSSetViewports(1, &splitScreenViewports_->at(splitScreenViewportIndex).viewport);

	//D3D11_VIEWPORT debug;
	//UINT numDebugs = 1;
	//devcon->RSGetViewports(&numDebugs, &debug);
}

HRESULT ManagementViewport::resize()
{
	HRESULT hr = S_OK;

	SAFE_DELETE(splitScreenViewports_);
	hr = init();

	return hr;
}

HRESULT ManagementViewport::init()
{
	splitScreenViewports_ = new std::vector<SplitScreenViewport>();

	unsigned int gridSizes[] = {1, 2, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
	unsigned int gridSize = 0;
	unsigned int index = 0;
	HRESULT hr = E_FAIL;

	while(FAILED(hr) && index<11)
	{
		if(winfo_->getNumViewports() <= gridSizes[index])
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

	//By this stage, number of viewports in X- and Y has been determined.
	//Initialize Winfo with this updated data:
	winfo_->init(
		winfo_->getScreenWidth(),
		winfo_->getScreenHeight(),
		winfo_->getNumViewports(),
		winfo_->getCSDispathX(),
		winfo_->getCSDispathY(),
		numViewportsX_,
		numViewportsY_);

	// Inform about change in splitscreen
	SEND_EVENT(&Event(EVENT_SPLITSCREEN_CHANGED));
	return hr;
}

HRESULT ManagementViewport::initViewportSingle()
{
	HRESULT hr = S_OK;

	if(itrSplitScreen.hasNext())
	{
		SplitScreenViewport ssViewport;
		ssViewport.ptr_splitScreen = itrSplitScreen.getNext(); 

		viewportWidth_	= winfo_->getScreenWidth();
		viewportHeight_ = winfo_->getScreenHeight();

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX	= 0;
		viewport.TopLeftY	= 0;
		viewport.Width		= static_cast<FLOAT>(viewportWidth_);
		viewport.Height		= static_cast<FLOAT>(viewportHeight_);
		viewport.MinDepth	= 0;
		viewport.MaxDepth	= 1;
		ssViewport.viewport = viewport;

		splitScreenViewports_->push_back(ssViewport);

		numViewportsX_ = 1;
		numViewportsY_ = 1;

		//Init Split-screen attribute
		ssViewport.ptr_splitScreen->ssWidth	= viewportWidth_;
		ssViewport.ptr_splitScreen->ssHeight	= viewportHeight_;

		ssViewport.ptr_splitScreen->ssTopLeftX = 0;
		ssViewport.ptr_splitScreen->ssTopLeftY = 0;
	}
	itrSplitScreen.resetIndex();

	return hr;
}
HRESULT ManagementViewport::initViewportDouble()
{
	HRESULT hr = S_FALSE;

	viewportWidth_	= winfo_->getScreenWidth();
	viewportHeight_ = winfo_->getScreenHeight() / 2;

	unsigned int ssCount = 0;
	while(itrSplitScreen.hasNext() && ssCount < 2)
	{
		SplitScreenViewport ssViewport;
		ssViewport.ptr_splitScreen = itrSplitScreen.getNext();

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.Width		= static_cast<FLOAT>(viewportWidth_);
		viewport.Height		= static_cast<FLOAT>(viewportHeight_) - borderSize_;
		viewport.MinDepth	= 0;
		viewport.MaxDepth	= 1;

		switch (ssCount)
		{
		case 0:
			viewport.TopLeftX	= 0;
			viewport.TopLeftY	= 0;
			break;
		case 1:
			viewport.TopLeftX	= 0;
			viewport.TopLeftY	= static_cast<FLOAT>(viewportHeight_) + borderSize_;
			break;
		}
		ssViewport.viewport = viewport;
		splitScreenViewports_->push_back(ssViewport);

		//Init split-screen attribute
		ssViewport.ptr_splitScreen->ssWidth	= static_cast<unsigned>(viewport.Width);
		ssViewport.ptr_splitScreen->ssHeight	= static_cast<unsigned>(viewport.Height);
		ssViewport.ptr_splitScreen->ssTopLeftX = static_cast<unsigned>(viewport.TopLeftX);
		ssViewport.ptr_splitScreen->ssTopLeftY = static_cast<unsigned>(viewport.TopLeftY);

		ssCount++;
	}
	itrSplitScreen.resetIndex();

	if(ssCount == 2)
	{
		numViewportsX_ = 1;
		numViewportsY_ = 2;

		hr = S_OK;
	}

	return hr;
}
HRESULT ManagementViewport::initViewportGrid(unsigned int gridSize)
{
	HRESULT hr = S_OK;

	unsigned int numGridColumns = static_cast<unsigned int>(sqrt(gridSize));

	unsigned int viewportTotalWidth		= winfo_->getScreenWidth()	- ((numGridColumns-1) * borderSize_);
	unsigned int viewportTotalHeight	= winfo_->getScreenHeight()	- ((numGridColumns-1) * borderSize_);
	viewportWidth_ = viewportTotalWidth / numGridColumns;
	viewportHeight_ = viewportTotalHeight / numGridColumns;

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(viewportWidth_);
	viewport.Height		= static_cast<FLOAT>(viewportHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	bool noMoreAttributes = false;
	for(unsigned int row = 0; row < numGridColumns && !noMoreAttributes; row++)
	{
		for(unsigned int column = 0; column < numGridColumns && !noMoreAttributes; column++)
		{
			if(itrSplitScreen.hasNext())
			{
				SplitScreenViewport ssViewport;
				ssViewport.ptr_splitScreen = itrSplitScreen.getNext();

				viewport.TopLeftX = static_cast<FLOAT>(column * (viewportWidth_ + borderSize_));
				viewport.TopLeftY = static_cast<FLOAT>(row * (viewportHeight_ + borderSize_));

				ssViewport.viewport = viewport;
				splitScreenViewports_->push_back(ssViewport);

				//Init split-screen attribute
				ssViewport.ptr_splitScreen->ssWidth	= static_cast<unsigned>(viewport.Width);
				ssViewport.ptr_splitScreen->ssHeight	= static_cast<unsigned>(viewport.Height);
				ssViewport.ptr_splitScreen->ssTopLeftX = static_cast<unsigned>(viewport.TopLeftX);
				ssViewport.ptr_splitScreen->ssTopLeftY = static_cast<unsigned>(viewport.TopLeftY);
			}
			else
				noMoreAttributes = true;
		}
	}
	itrSplitScreen.resetIndex();

	numViewportsX_ = numGridColumns;
	numViewportsY_ = numGridColumns;

	return hr;
}

unsigned int ManagementViewport::getViewportWidth() const
{
	return viewportWidth_;
}
unsigned int ManagementViewport::getViewportHeight() const
{
	return viewportHeight_;
}
unsigned int ManagementViewport::getNumViewportsX() const
{
	return numViewportsX_;
}
unsigned int ManagementViewport::getNumViewportsY() const
{
	return numViewportsY_;
}
std::vector<SplitScreenViewport>* ManagementViewport::getSplitScreenViewports()
{
	return splitScreenViewports_;
}