#ifndef XKILL_RENDERER_GBUFFER_H
#define XKILL_RENDERER_GBUFFER_H

#include <d3d11.h>

#include "d3dInterface.h"

//! Holds resources for each g-buffer.
/*!
Each GBuffer-object contains a ID3D11Texture2D*-, ID3D11ShaderResourceView*- 
and a ID3D11RenderTargetView*-object, along with a description of 
said g-buffer (texture dimensions, format and aliasing-settings).
	\ingroup xkill-renderer
	\sa ID3D11Texture2D
	\sa ID3D11ShaderResourceView
	\sa ID3D11RenderTargetView
*/
class GBuffer : public D3DInterface
{
public:
	//! Sets default state of GBuffer.
	/*!
	\param texWidth		Width of g-buffer texture.
	\param texHeight	Height of g-buffer texture.
	\param texAliasing	Anti-aliasing samples of texture.
	\param texFormat	Format of texture.
	\sa DXGI_FORMAT
	*/
	GBuffer(
		unsigned int	texWidth,
		unsigned int	texHeight,
		unsigned int	texAliasing,
		DXGI_FORMAT		texFormat);
	//! Clears memory and sets Gbuffer to default state.
	~GBuffer();

	//! Clears memory and sets Gbuffer to default state.
	void reset();

	HRESULT resize(ID3D11Device* device, unsigned int texWidth, unsigned int texHeight);

	//! Initializes g-buffer. Creates a texture-, shader resource view- and a render target view-object.
	/*!
	\param device DirectX Device pointer.
	\return Any error that might've occured.
	\sa initTex
	\sa initSRV
	\sa initRTV
	*/
	HRESULT init(ID3D11Device* device);

	ID3D11ShaderResourceView*	getSRV();
	ID3D11RenderTargetView*		getRTV();
private:
	//! Initializes the texture of GBuffer
	/*! Creates a description based on dimensions and formats sent to GBuffer via constructor and 
	creates a ID3D11Texture2D-object based on that description.
	\param device DirectX Device pointer.
	\return Any error that might've occured.
	*/
	HRESULT initTex(ID3D11Device* device);
	//! Initializes the shader resource view of GBuffer.
	/*! Creates a description based on dimensions and formats sent to GBuffer via constructor and creates
	a ID3D11ShaderResourceView based on texture created in initTex.
	\param device DirectX Device pointer.
	\return Any error that might've occured.
	\sa initTex
	*/
	HRESULT initSRV(ID3D11Device* device);
	//! Initializes the render target view of Gbuffer
	/*! Creates a description based on dimensions and formats sent to GBuffer via constructor and creates
	a ID3D11RenderTargetView based on texture created in initTex.
	\param device DirectX Device pointer.
	\return Any error that might've occured.
	\sa initTex
	*/
	HRESULT initRTV(ID3D11Device* device);

	unsigned int	texWidth_;		//!< Width of texture.
	unsigned int	texHeight_;		//!< Height of texture.
	unsigned int	texAliasing_;	//!< Anti-aliasing samples of texture.
	DXGI_FORMAT		texFormat_;		//!< Format of texture.

	ID3D11Texture2D*			tex_;	//!< Texture of GBuffer.
	ID3D11ShaderResourceView*	srv_;	//!< Handle to texture used by shader.
	ID3D11RenderTargetView*		rtv_;	//!< Used by shader to render to texture.
};

#endif