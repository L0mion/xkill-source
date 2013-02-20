#ifndef XKILL_RENDERER_BUFFER_SRV_H
#define XKILL_RENDERER_BUFFER_SRV_H

typedef long HRESULT;

enum DXGI_FORMAT;

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

#include "d3dInterface.h"

class Buffer_Srv : public D3DInterface
{
public:
	Buffer_Srv(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat);
	virtual ~Buffer_Srv();

	virtual void reset();

	HRESULT resize(
		ID3D11Device* device, 
		unsigned int texWidth, 
		unsigned int texHeight);

	virtual HRESULT init(ID3D11Device* device);

	ID3D11ShaderResourceView* getSRV() { return srv_; }
protected:
	ID3D11Texture2D*			tex_;	//!< Texture. What more do you want? Piss off.
	ID3D11ShaderResourceView*	srv_;	//!< Handle to texture used by shader.

	unsigned int	texWidth_;		//!< Width of texture.
	unsigned int	texHeight_;		//!< Height of texture.
	unsigned int	texAliasing_;	//!< Anti-aliasing samples of texture.
	DXGI_FORMAT		texFormat_;		//!< Format of texture.
private:
	virtual HRESULT initTex(ID3D11Device* device);	//!< Initializes texture with description specified in most-derived method.
	HRESULT initSRV(ID3D11Device* device);			//!< Initializes ShaderResourceView-element.
};

#endif //XKILL_RENDERER_BUFFER_SRV_H