#ifndef XKILL_RENDERER_SHADERVS_H
#define XKILL_RENDERER_SHADERVS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <shader.h>

//! Class derived from class Shader responsible for loading a vertex shader.
/*!
\ingroup xkill-renderer-shader
*/
class ShaderVS : public Shader
{
public:
	//! Sets ShaderVS to default state.
	ShaderVS();
	//! Releases all memory and returns ShaderVS to default state
	virtual ~ShaderVS();

	//! Releases all memory and returns ShaderVS to default state
	virtual void reset();
	//! Initializes Shader
	/*!
	\param device DirectX device pointer
	\param shaderPath File path to a precompiled shader.
	*/
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath);

	//! Sets Vertex Shader.
	/*!
	\param devcon DirectX Device Context pointer.
	*/
	virtual void set(ID3D11DeviceContext* devcon);
	//! Unsets Vertex Shader.
	/*!
	\param devcon DirectX Device Context pointer
	*/
	virtual void unset(ID3D11DeviceContext* devcon);
	
	ID3D11VertexShader* getVertexShader() const;

private:
	ID3D11VertexShader* vertexShader; //!< Contains vertex shader.
};

#endif //XKILL_RENDERER_SHADERVS_H