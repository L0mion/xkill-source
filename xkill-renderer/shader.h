#ifndef XKILL_RENDERER_SHADER_H
#define XKILL_RENDERER_SHADER_H

#include <d3d11.h>
#include <d3d10.h>
#include <d3dInterface.h>

#include <string>

//! Class responsible for loading a shader.
/*!
\ingroup xkill-renderer
*/

class Shader : public D3DInterface
{
public:
	//! Sets Shader to default state.
	Shader();
	//! Releases all memory and returns Shader to default state
	~Shader();

	//! Releases all memory and returns Shader to default state
	virtual void reset();
	//! Initializes Shader
	/*!
	\param device DirectX device pointer
	\param shaderPath File path to a precompiled shader.
	*/
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath) = 0;

	ID3D10Blob* getBlob();

protected:
	ID3D10Blob*	blob;		//!< DirectX blob object, contains compiled shader code.
	LPCWSTR		shaderPath; //!< Path to a precompiled shader. 
};

#endif //XKILL_RENDERER_SHADER_H