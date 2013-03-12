#ifndef XKILL_RENDERER_BUFFER_SRVRTVUAV_H
#define XKILL_RENDERER_BUFFER_SRVRTVUAV_H

typedef long HRESULT;

enum DXGI_FORMAT;

struct ID3D11UnorderedAccessView;
struct ID3D11Device;

#include "Buffer_SrvRtv.h"

/*
Give me these settings, please:
D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
D3D11_USAGE_DEFAULT
*/

class Buffer_SrvRtvUav : public Buffer_SrvRtv
{
public:
	Buffer_SrvRtvUav(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		unsigned int	texMips,
		DXGI_FORMAT		texFormat,
		UINT			texBindFlags,
		D3D11_USAGE		texUsage);
	virtual ~Buffer_SrvRtvUav();

	virtual void reset();

	virtual HRESULT init(ID3D11Device* device);

	ID3D11UnorderedAccessView* getUAV() { return uav_; }
protected:
private:
	HRESULT initUAV(ID3D11Device* device);

	ID3D11UnorderedAccessView* uav_;
};

#endif //XKILL_RENDERER_BUFFER_SRVRTVUAV_H