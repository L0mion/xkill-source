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
	LAYOUTID_POS_COLOR,
	LAYOUTID_POS_NORM_TEX_INSTANCED,
	LAYOUTID_POS_NORM_TEX_TAN_SKINNED,
	LAYOUTID_POS_NORM_TEX,
	LAYOUTID_POS_NORM_TEX_TAN_INSTANCED
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
	void unsetLayout(ID3D11DeviceContext*	devcon);

	void unsetAll(ID3D11DeviceContext* devcon);

private:
	HRESULT initShaders(ID3D11Device*			device);							//!< Initializes all shaders handled by FXManagement.
	HRESULT initDefaultVS(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes defaultVS.
	HRESULT initDefaultPS(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes defaultPS.
	HRESULT initDefaultDeferredVS(ID3D11Device* device, std::wstring shaderPath);	//!< Initializes defaultDeferredVS.
	HRESULT initDefaultDeferredPS(ID3D11Device* device, std::wstring shaderPath);	//!< Initializes defaultDeferredPS.
	HRESULT initDefaultCS(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes defaultCS.
	HRESULT initAnimationVS(ID3D11Device*		device,	std::wstring shaderPath);	//!< Initializes animationVS.
	HRESULT initAnimationPS(ID3D11Device*		device,	std::wstring shaderPath);	//!< Initializes animationPS.
	HRESULT initColorVS(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes color-shaders.
	HRESULT initColorPS(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes color-shaders.
	HRESULT initSpriteVS(ID3D11Device*			device, std::wstring shaderPath);	//!< Initializes sprite vertex shader.
	HRESULT initSpritePS(ID3D11Device*			device, std::wstring shaderPath);	//!< Initializes sprite pixel shader.
	HRESULT initVSPosNormTexTanInstanced(ID3D11Device* device, std::wstring shaderPath);
	HRESULT initPSNormalMap(ID3D11Device*		device, std::wstring shaderPath);

	HRESULT initILs(ID3D11Device* device);						//!< Initializes input-layouts.
	void initILManagement();									//!< Initializes helper-class IEDManagement.
	HRESULT initILPosColor(ID3D11Device* device);				//!< Initializes input layout of MeshVertex VertexPosColor.
	HRESULT initILPosNormTexInstanced(ID3D11Device* device);	//!< Initializes input layout of a standard three-component instanced vertex.
	HRESULT initILPosNormTexTanSkinned(ID3D11Device* device);	//!< Initializes input layout of MeshVertex VertexPosNormTexTanSkinned.
	HRESULT initILPosNormTex(ID3D11Device* device);				//!< Initializes input layout of MeshVertex VertexPosNormTex.
	HRESULT initILPosNormtexTanInstanced(ID3D11Device* device);

	Shader* getShaderFromID(ShaderID shaderID);

	ManagementIED* managementIED_;	//!< Helper class holding input element descriptions.
	bool debugShaders_;				//!< Decides if FXManagement should load debug or release configured shaders.

	ShaderVS*	defaultVS_;			//!< Default vertex shader.
	ShaderPS*	defaultPS_;			//!< Default pixel shader.
	ShaderVS*	defaultDeferredVS_;	//!< Default vertex shader used by the deferred renderer.
	ShaderPS*	defaultDeferredPS_;	//!< Default pixel shader used by the deferred renderer.
	ShaderCS*	defaultCS_;			//!< Default compute shader.
	ShaderVS*	animationVS_;		//!< Vertex shader used for animated meshes.
	ShaderPS*	animationPS_;		//!< Pixel shader used for animated meshes.
	ShaderVS*	colorVS_;
	ShaderPS*	colorPS_;
	ShaderVS*	spriteVS_;			//!< Vertex shader used for sprites.
	ShaderPS*	spritePS_;			//!< Pixel shader used for sprites.
	ShaderVS*	vsPosNormTexTanInstanced_;
	ShaderPS*	psNormalMap_;

	ID3D11InputLayout* ilPosColor_;					//!< Input layout specifying position and color.
	ID3D11InputLayout* ilPosNormTexInstanced_;		//!< Standard instanced input layout used in default vertex shader.
	ID3D11InputLayout* ilPosNormTexTanSkinned_;		//!< Input layout for the vertex type VertexPosNormTexTanSkinned.
	ID3D11InputLayout* ilPosNormTex_;				//!< Input layout for the vertex type VertexPosNormTex.
	ID3D11InputLayout* ilPosNormTexTanInstanced_;
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H