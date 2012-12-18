#ifndef XKILL_RENDERER_FXMANAGEMENT_H
#define XKILL_RENDERER_FXMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11DeviceContext;

#include "d3dInterface.h"
#include "ManagementIED.h"
#include "shaderVS.h"
#include "shaderPS.h"
#include "shaderCS.h"

enum LayoutID
{
	LAYOUTID_POS_NORM_TEX,
	LAYOUTID_POS_NORM_TEX_TAN_SKINNED
};

//! Class maintaing shaders and input layout.
/*!
\ingroup xkill-renderer
*/
class ManagementFX : public D3DInterface
{
public:
	ManagementFX(bool debugShaders);	//!< Sets FXManagement to default state.
	~ManagementFX();					//!< Releases all memory and returns FXManagement to defualt state.
	
	void reset(); //!< Releases all memory and returns FXManagement to defualt state.
	HRESULT init(ID3D11Device* device);	//!< Initializes FXManagement

	void setShader(ID3D11DeviceContext*		devcon,	ShaderID shaderID);
	void unsetShader(ID3D11DeviceContext*	devcon,	ShaderID shaderID);
	void setLayout(ID3D11DeviceContext*		devcon,	LayoutID layoutID);

private:
	HRESULT initShaders(ID3D11Device* device);										//!< Initializes all shaders handled by FXManagement.
	HRESULT initDefaultVS(ID3D11Device* device,			std::wstring shaderPath);	//!< Initializes defaultVS.
	HRESULT initDefaultPS(ID3D11Device* device,			std::wstring shaderPath);	//!< Initializes defaultPS.
	HRESULT initDefaultDeferredVS(ID3D11Device* device, std::wstring shaderPath);	//! Initializes defaultDeferredVS.
	HRESULT initDefaultDeferredPS(ID3D11Device* device, std::wstring shaderPath);	//! Initializes defaultDeferredPS.
	HRESULT initDefaultCS(ID3D11Device* device,			std::wstring shaderPath);	//! Initializes defaultCS.
	HRESULT initAnimationVS(ID3D11Device* device,		std::wstring shaderPath);	//! Initializes animationVS.
	HRESULT initAnimationPS(ID3D11Device* device,		std::wstring shaderPath);	//!< Initializes animationPS.

	HRESULT initILs(ID3D11Device* device);	//!< Initializes an input-layout for defaultVS.
	void initILManagement();
	HRESULT initILDefaultVSPosNormTex(ID3D11Device* device);
	HRESULT initILPosNormTexTanSkinned(ID3D11Device* device);

	Shader* getShaderFromID(ShaderID shaderID);

	ManagementIED* managementIED_;
	bool debugShaders_;				//!< Decides if FXManagement should load debug or release configured shaders.

	ShaderVS*	defaultVS_;			//!< Default vertex shader.
	ShaderPS*	defaultPS_;			//!< Default pixel shader.
	ShaderVS*	defaultDeferredVS_;	//!< Default vertex shader used by the deferred renderer.
	ShaderPS*	defaultDeferredPS_;	//!< Default pixel shader used by the deferred renderer.
	ShaderCS*	defaultCS_;			//!< Default compute shader.
	ShaderVS*	animationVS_;		//!< Vertex shader used for animated meshes.
	ShaderPS*	animationPS_;		//!< Pixel shader used for animated meshes.
	
	ID3D11InputLayout* ilPosNormTex_; //!< Standard input layout used in default vertex shader.
	ID3D11InputLayout* ilPosNormTexTanSkinned_; //!< Input layout for the vertex type VertexPosNormTexTanSkinned.
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H