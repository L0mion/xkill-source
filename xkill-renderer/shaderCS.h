#ifndef XKILL_RENDERER_SHADERCS_H
#define XKILL_RENDERER_SHADERCS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <shader.h>

//! Class derived from class Shader responsible for loading a compute shader.
/*!
\ingroup xkill-renderer-shader
*/
class ShaderCS : public Shader
{
public:
	//! Sets ShaderCS to default state.
	ShaderCS();
	//! Releases all memory and returns ShaderCS to default state.
	virtual ~ShaderCS();

	//! Releases all memory and returns ShaderCS to default state.
	virtual void reset();
	
	//! Initializes Shader.
	/*!
	\param device DirectX device pointer
	\param shaderPath File path to a precompiled shader.
	*/
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath);

	//! Sets Compute Shader.
	/*!
	\param devcon DirectX Device Context pointer.
	*/
	virtual void set(ID3D11DeviceContext* devcon);
	//! Unsets Compute Shader.
	/*!
	\param devcon DirectX Device Context pointer
	*/
	virtual void unset(ID3D11DeviceContext* devcon);

	ID3D11ComputeShader* getComputeShader() const;
private:
	ID3D11ComputeShader* computeShader; //!< Contains compute shader.

};

#endif //XKILL_RENDERER_SHADERCS_H