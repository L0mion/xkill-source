#ifndef XKILL_RENDERER_BUFFER_SRVRTV_H
#define XKILL_RENDERER_BUFFER_SRVRTV_H

typedef long HRESULT;

enum DXGI_FORMAT;

struct ID3D11RenderTargetView;
struct ID3D11Device;

#include "Buffer_Srv.h"

/*
Give me these settings, please:
D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
D3D11_USAGE_DEFAULT
*/

class Buffer_SrvRtv : public Buffer_Srv
{
public:
	Buffer_SrvRtv(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat,
		UINT			texBindFlags,
		D3D11_USAGE		texUsage);
	virtual ~Buffer_SrvRtv();

	virtual void reset();

	virtual HRESULT init(ID3D11Device* device);

	ID3D11RenderTargetView* getRTV() { return rtv_; }
protected:
	ID3D11RenderTargetView*	rtv_; //!< Used by shader to render to texture.
private:
	HRESULT initRTV(ID3D11Device* device);
};

#endif //XKILL_RENDERER_BUFFER_SRVRTV_H