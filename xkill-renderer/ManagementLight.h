#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>

#include <xkill-utilities/LightDesc.h>

#include "d3dInterface.h"
#include "LightPos.h"
#include "DataStreamSRV.h"

typedef long HRESULT;

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMFLOAT3;
};

struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

static const unsigned int LIGHT_SRV_REGISTER_DIR	= 6;
static const unsigned int LIGHT_SRV_REGISTER_POINT	= 7;
static const unsigned int LIGHT_SRV_REGISTER_SPOT	= 8;
static const unsigned int LIGHT_SRV_REGISTER_POS	= 9;

enum DLL_U LightBufferType
{ 
	LIGHTBUFFERTYPE_DIR			= 0, 
	LIGHTBUFFERTYPE_POINT		= 1, 
	LIGHTBUFFERTYPE_SPOT		= 2, 
	LIGHTBUFFERTYPE_POS_VIEW	= 3,
	LIGHTDESCTYPE_NA			= 4
};

//! Class for maintaining lights.
/*!
\ingroup xkill-renderer
*/
class ManagementLight : public D3DInterface
{
public:
	ManagementLight();	//!< Sets LightManagement to its default state.
	~ManagementLight();	//!< Releases all memory and returns LightManagement to its default state.

	void init();

	virtual void reset(); //!< Releases all memory and returns LightManagement to its default state.
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Updates all light-buffers with data from Attribute_Lights.
	
	void setLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightBufferType			bufferType, 
		unsigned int			shaderRegister); //!< Set the compute shader to use specified SRV.
	void unsetLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightBufferType			bufferType, 
		unsigned int			shaderRegister);

	//void transformLightViewSpacePoss(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 view);

	unsigned int getLightDirCurCount();
	unsigned int getLightPointCurCount();
	unsigned int getLightSpotCurCount();
private:
	void updateStreamDirLight();
	void updateStreamPointLight();
	void updateStreamSpotLight();
	//void updateStreamPosLight();

	DataStreamSRV<LightDescDir>*	streamDirLight_;
	DataStreamSRV<LightDescPoint>*	streamPointLight_;
	DataStreamSRV<LightDescSpot>*	streamSpotLight_;
	DataStreamSRV<Float3>*			streamPosLight_;
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H