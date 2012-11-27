#ifndef XKILL_RENDERER_SHADERPS_H
#define XKILL_RENDERER_SHADERPS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include "shader.h"


//! Class derived from class Shader responsible for loading a pixel shader.
/*!
\ingroup xkill-renderer-shader
*/
class ShaderPS : public Shader
{
public:
	//! Sets ShaderPS to default state.
	ShaderPS();
	//! Releases all memory and returns ShaderPS to default state
	virtual ~ShaderPS();

	//! Releases all memory and returns ShaderPS to default state
	virtual void reset();
	//! Initializes Shader
	/*!
	\param device DirectX device pointer
	\param shaderPath File path to a precompiled shader.
	*/
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath);
	
	//! Sets Pixel Shader.
	/*!
	\param devcon DirectX Device Context pointer
	*/
	virtual void set(ID3D11DeviceContext* devcon);
	//! Unsets Pixel Shader.
	/*!
	\param devcon DirectX Device Context pointer
	*/
	virtual void unset(ID3D11DeviceContext* devcon);

	ID3D11PixelShader* getPixelShader();

private:
	ID3D11PixelShader* pixelShader_; //!< Contains pixel shader
};

#endif //XKILL_RENDERER_SHADERPS_H