#ifndef XKILL_RENDERER_FXMANAGEMENT_H
#define XKILL_RENDERER_FXMANAGEMENT_H

#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>

#include <d3dInterface.h>

//! Class maintaing shaders and input layout.
/*!
\ingroup xkill-renderer
*/
class FXManagement : public D3DInterface
{
public:
	//! Sets FXManagement to default state.
	FXManagement();
	//! Releases all memory and returns FXManagement to defualt state.
	~FXManagement();
	
	//! Releases all memory and returns FXManagement to defualt state.
	void reset();

	//! Initializes FXManagement
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa initShaders
	\sa initInputLayout
	*/
	HRESULT init(ID3D11Device* device);

	ID3D11VertexShader*	getDefaultVS()			const;
	ID3D11PixelShader*	getDefaultPS()			const;
	ID3D11VertexShader* getDefaultDeferredVS()	const;
	ID3D11PixelShader*	getDefaultDeferredPS()	const;

	ID3D11InputLayout* getInputLayout() const;

private:
	//! Initializes all shaders handled by FXManagement.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa initDefaultVS
	\sa initDefaultPS
	\sa initDefaultDeferredVS
	\sa initDefaultDeferredPS
	*/
	HRESULT initShaders(ID3D11Device* device);
	//! Initializes defaultVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultVS
	*/
	HRESULT initDefaultVS(ID3D11Device* device);
	//! Initializes defaultPS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultPS
	*/
	HRESULT initDefaultPS(ID3D11Device* device);
	//! Initializes defaultDeferredVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultDeferredVS
	*/
	HRESULT initDefaultDeferredVS(ID3D11Device* device);
	//! Initializes defaultDeferredPS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultDeferredPS
	*/
	HRESULT initDefaultDeferredPS(ID3D11Device* device);
	//! Initializes an input-layout for defaultVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa inputLayout
	*/
	HRESULT initInputLayout(ID3D11Device* device);

	ID3D11VertexShader*	defaultVS;			//!< Default vertex shader.
	ID3D11PixelShader*	defaultPS;			//!< Default pixel shader.
	ID3D11VertexShader*	defaultDeferredVS;	//!< Default vertex shader used by the deferred renderer.
	ID3D11PixelShader*	defaultDeferredPS;	//!< Default pixel shader used by the deferred renderer.
	
	ID3D10Blob* blobDefaultVS;			//!< Used to load pre-compiled default vertex shader.
	ID3D10Blob* blobDefaultPS;			//!< Used to load pre-compiled default pixel shader.
	ID3D10Blob* blobDefaultDeferredVS;	//!< Used to load pre-compiled default deferred vertex shader.
	ID3D10Blob* blobDefaultDeferredPS;	//!< Used to load pre-compiled default deferred pixel shader.

	ID3D11InputLayout* inputLayout; //!< Standard input layout used in default vertex shader.
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H