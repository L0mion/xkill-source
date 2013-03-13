#ifndef XKILL_RENDERER_SHADERGS_H
#define XKILL_RENDERER_SHADERGS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include "shader.h"

class ShaderGS : public Shader
{
public:
	ShaderGS();
	virtual ~ShaderGS();

	virtual void reset();
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath);
	virtual void set(ID3D11DeviceContext* devcon);

	ID3D11GeometryShader* getGeometryShader();
protected:
private:
	ID3D11GeometryShader* geometryShader_;
};

#endif //XKILL_RENDERER_SHADERGS_H