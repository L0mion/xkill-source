#ifndef XKILL_RENDERER_D3DDEBUG_H
#define XKILL_RENDERER_D3DDEBUG_H

#include <d3d11.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <DXGI.h> 
#include <Initguid.h> 
#include <DXGIDebug.h>
#include <D3D11SDKLayers.h>
#endif //DEBUG || _DEBUG

#include <d3dInterface.h>

class D3DDebug : public D3DInterface
{
public:
	D3DDebug();
	~D3DDebug();

	void cleanUp();
	HRESULT init(ID3D11Device* device);

	HRESULT reportLiveDeviceObjects();
	HRESULT reportLiveObjects();
private:
	#if defined(DEBUG) || defined(_DEBUG)
	IDXGIFactory1*	dxgiFactory;
	ID3D11Debug*	d3d11Debug;
	IDXGIDebug*		dxgiDebug;
	IDXGIInfoQueue* dxgiInfoQueue;
	#endif //DEBUG || _DEBUG
};

#endif //XKILL_RENDERER_D3DDEBUG_H