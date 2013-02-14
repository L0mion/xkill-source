#ifndef XKILL_RENDERER_BUFFER_SRVRTVUAV_H
#define XKILL_RENDERER_BUFFER_SRVRTVUAV_H

#include "Buffer_SrvRtv.h"

class Buffer_SrvRtvUav : public Buffer_SrvRtv
{
public:
	Buffer_SrvRtvUav(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat,
		bool			maintainGivenResolution);
	~Buffer_SrvRtvUav();

	void reset();

	HRESULT init(ID3D11Device* device);
	HRESULT initTex(ID3D11Device* device);
	HRESULT initUAV(ID3D11Device* device);

	ID3D11UnorderedAccessView* getUAV();
protected:
private:
	ID3D11UnorderedAccessView* uav_;
};

#endif //XKILL_RENDERER_BUFFER_SRVRTVUAV_H