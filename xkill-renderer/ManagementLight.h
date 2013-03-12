#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>
#include <map>

#include <xkill-utilities/LightDesc.h>

#include "d3dInterface.h"
#include "LightPos.h"
#include "DataStreamSRV.h"
#include "ViewportData.h"

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

class CameraLights;

static const unsigned int LIGHT_SRV_REGISTER_DIR		= 7;
static const unsigned int LIGHT_SRV_REGISTER_POINT		= 8;
static const unsigned int LIGHT_SRV_REGISTER_POS		= 9;
static const unsigned int LIGHT_SRV_REGISTER_DIRECTION	= 10;

enum DLL_U LightBufferType
{ 
	LIGHTBUFFERTYPE_DIR			= 0, 
	LIGHTBUFFERTYPE_POINT		= 1, 
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
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon, std::vector<ViewportData> vpDatas); //!< Updates all light-buffers with data from Attribute_Lights.
	
	void setLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightBufferType			bufferType, 
		unsigned int			shaderRegister); //!< Set the compute shader to use specified SRV.
	void unsetLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightBufferType			bufferType, 
		unsigned int			shaderRegister);
	void setPosDirSRVCS(
		ID3D11DeviceContext* devcon,
		unsigned int camIndex,
		unsigned int shaderRegisterPos,
		unsigned int shaderRegisterDir);
	void unsetPosDirSRVCS(
		ID3D11DeviceContext* devcon,
		unsigned int camIndex,
		unsigned int shaderRegisterPos,
		unsigned int shaderRegisterDir);

	unsigned int getNumPoints(unsigned int camIndex);

	unsigned int getLightDirCurCount();
	unsigned int getLightPointCurCount();
private:
	void updateStreamDirLight(std::vector<ViewportData> vpDatas);
	void updateStreamPointLight(std::vector<ViewportData> vpDatas);

	void addDirLightDir(unsigned int camIndex, DirectX::XMVECTOR dir, DirectX::XMFLOAT4X4 view);
	void addPointLightPos(unsigned int camIndex, DirectX::XMVECTOR pos, DirectX::XMFLOAT4X4 view);

	DataStreamSRV<LightDescDir>*	streamDirLight_;
	DataStreamSRV<LightDescPoint>*	streamPointLight_;

	std::map<
		unsigned int,
		CameraLights*> cameraLightsMap_;
	CameraLights* getCameraLightsFromCameraIndex(unsigned int camIndex);
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H