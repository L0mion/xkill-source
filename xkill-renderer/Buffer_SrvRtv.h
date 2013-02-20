#ifndef XKILL_RENDERER_BUFFER_SRVRTV_H
#define XKILL_RENDERER_BUFFER_SRVRTV_H

typedef long HRESULT;

enum DXGI_FORMAT;

struct ID3D11RenderTargetView;
struct ID3D11Device;

#include "Buffer_Srv.h"

class Buffer_SrvRtv : public Buffer_Srv
{
public:
	Buffer_SrvRtv(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat);
	virtual ~Buffer_SrvRtv();

	virtual void reset();

	virtual HRESULT init(ID3D11Device* device);

	ID3D11RenderTargetView* getRTV() { return rtv_; }
protected:
	ID3D11RenderTargetView*	rtv_; //!< Used by shader to render to texture.
private:
	virtual HRESULT initTex(ID3D11Device* device);
	HRESULT initRTV(ID3D11Device* device);
};

#endif //XKILL_RENDERER_BUFFER_SRVRTV_H