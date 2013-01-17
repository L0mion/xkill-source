#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>

#include <xkill-utilities/LightDesc.h>

#include "d3dInterface.h"

typedef long HRESULT;

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMFLOAT3;
};

struct ID3D11UnorderedAccesView;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

static const unsigned int LIGHT_START_MAX_COUNT_DIR		= 5; //Can't be zero as one may not create a naught-sized vector.
static const unsigned int LIGHT_START_MAX_COUNT_POINT	= 5; //Can't be zero as one may not create a naught-sized vector.
static const unsigned int LIGHT_START_MAX_COUNT_SPOT	= 5; //Can't be zero as one may not create a naught-sized vector.

static const unsigned int POS_START_MAX_COUNT = LIGHT_START_MAX_COUNT_POINT + LIGHT_START_MAX_COUNT_SPOT;

static const unsigned int LIGHT_SRV_REGISTER_DIR	= 4;
static const unsigned int LIGHT_SRV_REGISTER_POINT	= 5;
static const unsigned int LIGHT_SRV_REGISTER_SPOT	= 6;
static const unsigned int LIGHT_SRV_REGISTER_POS	= 7;

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

	virtual void reset(); //!< Releases all memory and returns LightManagement to its default state.
	HRESULT init(ID3D11Device* device); //!< Initializes LightManagement.
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Updates all light-buffers with data from Attribute_Lights.
	
	void setLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightBufferType			bufferType, 
		unsigned int			shaderRegister); //!< Set the compute shader to use specified SRV.

	void setLightViewSpacePoss(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 view);

	//void transformPointPossToViewSpace(DirectX::XMFLOAT4X4 view);
	//void transformSpotPossToViewSpace(DirectX::XMFLOAT4X4 view);

	unsigned int getLightDirCurCount()		const;
	unsigned int getLightPointCurCount()	const;
	unsigned int getLightSpotCurCount()		const;
private:
	HRESULT initLightDir(ID3D11Device* device);			//!< Initializes directional light-buffer and srv, if lightDirCurCount > 0
	HRESULT initLightDirBuffer(ID3D11Device* device);	//!< Initializes directional light-buffer.
	HRESULT initLightDirSRV(ID3D11Device* device);		//!< Initializes directional srv.

	HRESULT initLightPoint(ID3D11Device* device);		//!< Initializes pointlight light-buffer and srv, if lightPointCurCount > 0
	HRESULT initLightPointBuffer(ID3D11Device* device);	//!< Initializes pointlight light-buffer.
	HRESULT initLightPointSRV(ID3D11Device* device);	//!< Initializes pointlight srv.

	HRESULT initLightSpot(ID3D11Device* device);		//!< Initializes spotlight light-buffer and srv, if lightSpotCurCount > 0
	HRESULT initLightSpotBuffer(ID3D11Device* device);	//!< Initializes spotlight light-buffer.
	HRESULT initLightSpotSRV(ID3D11Device* device);		//!< Initializes spotlight srv.

	HRESULT initLightPos(ID3D11Device* device);
	HRESULT initLightPosBuffer(ID3D11Device* device);
	HRESULT initLightPosSRV(ID3D11Device* device);

	void updateLightDir(ID3D11Device* device, ID3D11DeviceContext* devcon);		//!< Updated directional light-buffer with data from Attributes_LightDir.
	void updateLightPoint(ID3D11Device* device, ID3D11DeviceContext* devcon);	//!< Updated pointlight light-buffer with data from Attributes_LightPoint.
	void updateLightSpot(ID3D11Device* device, ID3D11DeviceContext* devcon);	//!< Updated spotlight light-buffer with data from Attributes_LightSpot.
	void updateLightPos(ID3D11Device* device);

	HRESULT increaseBufferCapacity(ID3D11Device* device, LightBufferType bufferType);		//!< Grows specified light-buffer capacity, allowing for more lights of that type to be sent to shader.
	HRESULT updateLightBuffers(ID3D11DeviceContext* devcon, LightBufferType bufferType);	//!< Maps new data to specified light-buffer.

	unsigned int lightDirMaxCount_;					//!< Number of directional light slots being bound to shader. 
	unsigned int lightDirCurCount_;					//!< Number of currently valid directional lights being sent to shader.
	std::vector<LightDescDir>	lightDirs_;			//!< Light-descriptions copied from Attributes_LightDir. These are then bound to buffer.
	ID3D11Buffer*				lightDirBuffer_;	//!< Buffer containing directional lights.
	ID3D11ShaderResourceView*	lightDirSRV_;		//!< SRV bound to directional lights.
	
	unsigned int lightPointMaxCount_;				//!< Number of pointlight slots being bound to shader.
	unsigned int lightPointCurCount_;				//!< Number of currently valid pointlights being sent to shader.
	std::vector<LightDescPoint> lightPoints_;		//!< Light-descriptions copied from Attributes_LightPoint. These are then bound to buffer.
	ID3D11Buffer*				lightPointBuffer_;	//!< Buffer containing pointlights.
	ID3D11ShaderResourceView*	lightPointSRV_;		//!< SRV bound to pointlights.

	unsigned int lightSpotMaxCount_;				//!< Number of spotlight slots being bound to shader.
	unsigned int lightSpotCurCount_;				//!< Number of currently valid spotlights being sent to shader.
	std::vector<LightDescSpot>	lightSpots_;		//!< Light-descriptions copied from Attributes_LightSpot. These are then bound to buffer.
	ID3D11Buffer*				lightSpotBuffer_;	//!< Buffer containing spotlights.
	ID3D11ShaderResourceView*	lightSpotSRV_;		//!< SRV bound to spotlights.

	unsigned int lightPosMaxCount_;
	unsigned int lightPosCurCount_;
	std::vector<Float3>			lightPossView_;
	std::vector<Float3>			lightPoss_;
	ID3D11Buffer*				lightPosViewBuffer_;
	ID3D11ShaderResourceView*	lightPosViewSRV_;

	std::vector<Attribute_Position>* attributesPosition_; //!< Holds positional data. Is fetched only once.
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H