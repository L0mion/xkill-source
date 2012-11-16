#ifndef XKILL_RENDERER_FXMANAGEMENT_H
#define XKILL_RENDERER_FXMANAGEMENT_H

#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>

class FXManagement
{
public:
	FXManagement();
	~FXManagement();

	HRESULT init(ID3D11Device* device);

	ID3D11VertexShader* getDefaultVertexShader() const;
	ID3D11PixelShader* getDefaultPixelShader() const;

private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initDefaultVertexShader(ID3D11Device* device);
	HRESULT initDefaultPixelShader(ID3D11Device* device);
	HRESULT initInputLayout(ID3D11Device* device);

	ID3D11VertexShader* defaultVertexShader;
	ID3D11PixelShader* defaultPixelShader;
	
	ID3D10Blob* defaultVS;
	ID3D10Blob* defaultPS;
	ID3D10Blob* error;

	ID3D11InputLayout* inputLayout;

};

#endif //XKILL_RENDERER_FXMANAGEMENT_H