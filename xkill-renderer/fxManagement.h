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

	ID3D11VertexShader* getDefaultVS() const;
	ID3D11PixelShader* getDefaultPS() const;
	ID3D11PixelShader* getDefaultDeferredPS() const;

private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initDefaultVS(ID3D11Device* device);
	HRESULT initDefaultPS(ID3D11Device* device);
	HRESULT initDefaultDeferredPS(ID3D11Device* device);
	HRESULT initInputLayout(ID3D11Device* device);

	ID3D11VertexShader* defaultVS;
	ID3D11PixelShader* defaultPS;
	ID3D11PixelShader* defaultDeferredPS;
	
	ID3D10Blob* blobDefaultVS;
	ID3D10Blob* blobDefaultPS;
	ID3D10Blob* blobDefaultDeferredPS;
	ID3D10Blob* blobError;

	ID3D11InputLayout* inputLayout;

};

#endif //XKILL_RENDERER_FXMANAGEMENT_H