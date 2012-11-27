#ifndef XKILL_RENDERER_VIEWPORTMANAGEMENT_H
#define XKILL_RENDERER_VIEWPORTMANAGEMENT_H

#include <vector>
#include <d3d11.h>

#include "d3dInterface.h"

class ViewportManagement : public D3DInterface
{
public:
	ViewportManagement(unsigned int numViewports,
						unsigned int viewportWidth,
						unsigned int viewportHeight,
						unsigned int screenWidth,
						unsigned int screenHeight);
	~ViewportManagement();

	virtual void reset();
	
	void setViewport(ID3D11DeviceContext* devcon, unsigned int index);

	HRESULT init();

private:

	HRESULT initViewportSingle();
	HRESULT initViewportDouble();
	HRESULT initViewportGrid(unsigned int gridSize);

	unsigned int numViewports_;
	unsigned int viewportWidth_;
	unsigned int viewportHeight_;
	unsigned int screenWidth_;
	unsigned int screenHeight_;

	std::vector<D3D11_VIEWPORT>* viewports;
};

#endif //XKILL_RENDERER_VIEWPORTMANAGEMENT_H