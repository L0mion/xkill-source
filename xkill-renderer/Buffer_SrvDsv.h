#ifndef XKILL_RENDERER_BUFFER_SRVDSV_H
#define XKILL_RENDERER_BUFFER_SRVDSV_H

typedef long HRESULT;

enum DXGI_FORMAT;

struct ID3D11Device;
struct ID3D11DepthStencilView;

#include "Buffer_Srv.h"

/*
Give me these settings, please:
texFormat: DXGI_FORMAT_R24G8_TYPELESS
texBindFlags: D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
texUsage: D3D11_USAGE_DEFAULT

dsvFormat: DXGI_FORMAT_D24_UNORM_S8_UINT

srvFormat: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
*/

class Buffer_SrvDsv : public Buffer_Srv
{
public:
	Buffer_SrvDsv(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat,
		UINT			texBindFlags,
		D3D11_USAGE		texUsage,

		DXGI_FORMAT srvFormat,
		
		DXGI_FORMAT dsvFormat);
	virtual ~Buffer_SrvDsv();

	virtual void reset();

	virtual HRESULT init(ID3D11Device* device);

	ID3D11DepthStencilView* getDSV() const { return dsv_; }
protected:
	ID3D11DepthStencilView* dsv_;
private:
	HRESULT initDSV(ID3D11Device* device);

	//DSV Description:
	DXGI_FORMAT dsvFormat_;
};

#endif //XKILL_RENDERER_BUFFER_SRVDSV_H