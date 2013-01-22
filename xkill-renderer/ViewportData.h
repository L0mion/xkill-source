#ifndef XKILL_RENDERER_VIEWPORTDATA_H
#define XKILL_RENDERER_VIEWPORTDATA_H

#include <DirectXMath.h>

struct ViewportData
{
	//unsigned int camIndex; 

	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 viewInv;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 projInv;
	DirectX::XMFLOAT3	eyePos;
	unsigned int		viewportTopX;
	unsigned int		viewportTopY;
	unsigned int		zNear;
	unsigned int		zFar;
};

#endif //XKILL_RENDERER_VIEWPORTDATA_H