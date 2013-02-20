#ifndef XKILL_RENDERER_BUFFER_SRV_H
#define XKILL_RENDERER_BUFFER_SRV_H

typedef long HRESULT;
typedef unsigned int UINT;

enum DXGI_FORMAT;
enum D3D11_USAGE;

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
		DXGI_FORMAT		texFormat,
		UINT			texBindFlags,
		D3D11_USAGE		texUsage);
	Buffer_Srv(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat,
		UINT			texBindFlags,
		D3D11_USAGE		texUsage,
		
		DXGI_FORMAT srvFormat);
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

	unsigned int getTexWidth()		const { return texWidth_;		}
	unsigned int getTexHeight()		const { return texHeight_;		}
	unsigned int getTexAliasing()	const { return texAliasing_;	}
	DXGI_FORMAT getTexFormat()		const { return texFormat_;		}
private:
	virtual HRESULT initTex(ID3D11Device* device);	//!< Initializes texture with description specified in most-derived method.
	HRESULT initSRV(ID3D11Device* device);			//!< Initializes ShaderResourceView-element.

	//Texture Description:
	unsigned int	texWidth_;		//!< Width of texture.
	unsigned int	texHeight_;		//!< Height of texture.
	unsigned int	texAliasing_;	//!< Anti-aliasing samples of texture.
	DXGI_FORMAT		texFormat_;		//!< Format of texture.
	UINT			texBindFlags_;	//!< Bindflags. Such as D3D11_BIND_DEPTH_STENCIL or D3D11_BIND_SHADER_RESOURCE.
	D3D11_USAGE		texUsage_;		//!< Usage, defines how memory ought to be handled, essentially.

	//SRV Description:
	DXGI_FORMAT srvFormat_;
};

#endif //XKILL_RENDERER_BUFFER_SRV_H