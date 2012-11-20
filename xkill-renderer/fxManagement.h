#ifndef XKILL_RENDERER_FXMANAGEMENT_H
#define XKILL_RENDERER_FXMANAGEMENT_H

#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>

#include <d3dInterface.h>

class FXManagement : public D3DInterface
{
public:
	FXManagement();
	~FXManagement();
	
	void cleanUp();

	HRESULT init(ID3D11Device* device);

	ID3D11VertexShader*	getDefaultVS()			const;
	ID3D11PixelShader*	getDefaultPS()			const;
	ID3D11VertexShader* getDefaultDeferredVS()	const;
	ID3D11PixelShader*	getDefaultDeferredPS()	const;

	ID3D11InputLayout* getInputLayout() const;

private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initDefaultVS(ID3D11Device* device);
	HRESULT initDefaultPS(ID3D11Device* device);
	HRESULT initDefaultDeferredVS(ID3D11Device* device);
	HRESULT initDefaultDeferredPS(ID3D11Device* device);
	HRESULT initInputLayout(ID3D11Device* device);

	ID3D11VertexShader*	defaultVS;
	ID3D11PixelShader*	defaultPS;
	ID3D11VertexShader*	defaultDeferredVS;
	ID3D11PixelShader*	defaultDeferredPS;
	
	ID3D10Blob* blobDefaultVS;
	ID3D10Blob* blobDefaultPS;
	ID3D10Blob* blobDefaultDeferredVS;
	ID3D10Blob* blobDefaultDeferredPS;

	ID3D11InputLayout* inputLayout;
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H