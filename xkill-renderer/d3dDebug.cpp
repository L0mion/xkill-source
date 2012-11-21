#include <renderingUtilities.h>

#include <d3dDebug.h>

D3DDebug::D3DDebug()
{
	#if defined(DEBUG) || defined(_DEBUG)
	dxgiFactory		= nullptr;
	d3d11Debug		= nullptr;
	dxgiDebug		= nullptr;
	dxgiInfoQueue	= nullptr;
	#endif //DEBUG || _DEBUG
}
D3DDebug::~D3DDebug()
{
	#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(dxgiFactory);
	SAFE_RELEASE(d3d11Debug);
	SAFE_RELEASE(dxgiDebug);
	SAFE_RELEASE(dxgiInfoQueue);
	#endif //DEBUG || _DEBUG
}

void D3DDebug::reset()
{
	#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(dxgiFactory);
	SAFE_RELEASE(d3d11Debug);
	SAFE_RELEASE(dxgiDebug);
	SAFE_RELEASE(dxgiInfoQueue);
	#endif //DEBUG || _DEBUG
}
HRESULT D3DDebug::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&dxgiFactory));
	if(hr == S_OK)
	{
		typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
		HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll"); 
		fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
		
		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&dxgiDebug);
		DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&dxgiInfoQueue);

		hr = device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3d11Debug));
	}
	#endif //DEBUG || _DEBUG

	return hr;
}

HRESULT D3DDebug::reportLiveDeviceObjects()
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	hr = d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	#endif //DEBUG || _DEBUG

	return hr;
}
HRESULT D3DDebug::reportLiveObjects()
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	hr = dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	#endif //DEBUG || _DEBUG

	return hr;
}