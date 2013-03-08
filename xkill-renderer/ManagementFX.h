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

enum ShaderID
{
	SHADERID_VS_POS_NORM_TEX_INSTANCE,
	SHADERID_VS_ANIMATION,
	SHADERID_VS_COLOR,
	SHADERID_VS_SPRITE,
	SHADERID_VS_POS_NORM_TEX_TAN_INSTANCE,
	SHADERID_VS_SCREENQUAD,

	SHADERID_PS_DEFAULT,
	SHADERID_PS_ANIMATION,
	SHADERID_PS_COLOR,
	SHADERID_PS_SPRITE,
	SHADERID_PS_NORMALMAP,
	SHADERID_PS_DOWNSAMPLE,
	SHADERID_PS_BUILD_SHADOWMAP_POS_NORM_TEX,

	SHADERID_CS_LIGHTING,
	SHADERID_CS_BLUR_HORZ,
	SHADERID_CS_BLUR_VERT,
	SHADERID_CS_BLUR_BILATERAL_HORZ,
	SHADERID_CS_BLUR_BILATERAL_VERT,
	SHADERID_CS_SSAO
};

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
	HRESULT initShaders(ID3D11Device* device); //!< Initializes all shaders handled by FXManagement.
	
	HRESULT initVSPosNormTexInstanced(ID3D11Device*		device,	std::wstring shaderPath);	//!< Initializes defaultVS.
	HRESULT initVSAnimation(ID3D11Device*				device,	std::wstring shaderPath);	//!< Initializes animationVS.
	HRESULT initVSColor(ID3D11Device*					device,	std::wstring shaderPath);	//!< Initializes color-shaders.
	HRESULT initVSSprite(ID3D11Device*					device, std::wstring shaderPath);	//!< Initializes sprite vertex shader.
	HRESULT initVSPosNormTexTanInstanced(ID3D11Device*	device, std::wstring shaderPath);
	HRESULT initVSScreenQuad(ID3D11Device*				device, std::wstring shaderPath);

	HRESULT initPSDefault(ID3D11Device*				device,	std::wstring shaderPath);	//!< Initializes defaultPS.
	HRESULT initPSColor(ID3D11Device*				device,	std::wstring shaderPath);	//!< Initializes color-shaders.
	HRESULT initPSAnimation(ID3D11Device*			device,	std::wstring shaderPath);	//!< Initializes animationPS.
	HRESULT initPSSprite(ID3D11Device*				device, std::wstring shaderPath);	//!< Initializes sprite pixel shader.
	HRESULT initPSNormalMap(ID3D11Device*			device, std::wstring shaderPath);
	HRESULT initPSDownSample(ID3D11Device*			device, std::wstring shaderPath);
	HRESULT initPSBuildShadowMapPosNormTex(ID3D11Device*	device, std::wstring shaderPath);

	HRESULT initCSLighting(	ID3D11Device* device, std::wstring shaderPath);	//!< Initializes defaultCS.
	HRESULT initCSBlurHorz(	ID3D11Device* device, std::wstring shaderPath);
	HRESULT initCSBlurVert(	ID3D11Device* device, std::wstring shaderPath);
	HRESULT initCSBlurBilateralHorz(	ID3D11Device* device, std::wstring shaderPath);
	HRESULT initCSBlurBilateralVert(	ID3D11Device* device, std::wstring shaderPath);
	HRESULT initCSSSAO(		ID3D11Device* device, std::wstring shaderPath);

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

	ShaderVS*	vsPosNormTexInstanced_;			//!< Default vertex shader.
	ShaderVS*	vsAnimation_;		//!< Vertex shader used for animated meshes.
	ShaderVS*	vsColor_;
	ShaderVS*	vsSprite_;			//!< Vertex shader used for sprites.
	ShaderVS*	vsPosNormTexTanInstanced_;
	ShaderVS*	vsScreenQuad_;

	ShaderPS*	psDefault_;			//!< Default pixel shader.
	ShaderPS*	psAnimation_;		//!< Pixel shader used for animated meshes.
	ShaderPS*	psColor_;
	ShaderPS*	psSprite_;			//!< Pixel shader used for sprites.
	ShaderPS*	psNormalMap_;
	ShaderPS*	psDownSample_;
	ShaderPS*	psBuildShadowMapPosNormTex_;

	ShaderCS* csLighting_; //!< Default compute shader.
	ShaderCS* csBlurHorz_;
	ShaderCS* csBlurVert_;
	ShaderCS* csBlurBilateralHorz_;
	ShaderCS* csBlurBilateralVert_;
	ShaderCS* csSSAO_;

	ID3D11InputLayout* ilPosColor_;					//!< Input layout specifying position and color.
	ID3D11InputLayout* ilPosNormTexInstanced_;		//!< Standard instanced input layout used in default vertex shader.
	ID3D11InputLayout* ilPosNormTexTanSkinned_;		//!< Input layout for the vertex type VertexPosNormTexTanSkinned.
	ID3D11InputLayout* ilPosNormTex_;				//!< Input layout for the vertex type VertexPosNormTex.
	ID3D11InputLayout* ilPosNormTexTanInstanced_;
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H