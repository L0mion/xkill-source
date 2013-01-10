#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>

#include <xkill-utilities/LightDesc.h>

#include "d3dInterface.h"

typedef long HRESULT;

struct ID3D11UnorderedAccesView;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

static const unsigned int LIGHT_START_MAX_COUNT_DIR		= 5; //Can't be zero as one may not create a naught-sized vector.
static const unsigned int LIGHT_START_MAX_COUNT_POINT	= 5; //Can't be zero as one may not create a naught-sized vector.
static const unsigned int LIGHT_START_MAX_COUNT_SPOT	= 5; //Can't be zero as one may not create a naught-sized vector.

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
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Updates light-buffer with data from Attribute_Lights.
	
	void setLightSRVCS(
		ID3D11DeviceContext*	devcon, 
		LightDescType			lightType, 
		unsigned int			shaderRegister); //!< Set the compute shader to use lightSRV_

	unsigned int getLightDirCurCount()		const;
	unsigned int getLightPointCurCount()	const;
	unsigned int getLightSpotCurCount()		const;
private:
	HRESULT initLightDir(ID3D11Device* device);
	HRESULT initLightDirBuffer(ID3D11Device* device);
	HRESULT initLightDirSRV(ID3D11Device* device);

	HRESULT initLightPoint(ID3D11Device* device);
	HRESULT initLightPointBuffer(ID3D11Device* device);
	HRESULT initLightPointSRV(ID3D11Device* device);

	HRESULT initLightSpot(ID3D11Device* device);
	HRESULT initLightSpotBuffer(ID3D11Device* device);
	HRESULT initLightSpotSRV(ID3D11Device* device);

	void updateLightDir(ID3D11Device* device, ID3D11DeviceContext* devcon);
	void updateLightPoint(ID3D11Device* device, ID3D11DeviceContext* devcon);
	void updateLightSpot(ID3D11Device* device, ID3D11DeviceContext* devcon);

	HRESULT increaseLightCapacity(ID3D11Device* device, LightDescType lightType);
	HRESULT updateLightBuffers(ID3D11DeviceContext* devcon, LightDescType lightType);

	unsigned int lightDirMaxCount_;
	unsigned int lightDirCurCount_;
	std::vector<LightDescDir>	lightDirs_;
	ID3D11Buffer*				lightDirBuffer_;
	ID3D11ShaderResourceView*	lightDirSRV_;
	
	unsigned int lightPointMaxCount_;
	unsigned int lightPointCurCount_;
	std::vector<LightDescPoint> lightPoints_;
	ID3D11Buffer*				lightPointBuffer_;
	ID3D11ShaderResourceView*	lightPointSRV_;

	unsigned int lightSpotMaxCount_;
	unsigned int lightSpotCurCount_;
	std::vector<LightDescSpot>	lightSpots_;
	ID3D11Buffer*				lightSpotBuffer_;
	ID3D11ShaderResourceView*	lightSpotSRV_;

	std::vector<Attribute_Position>* attributesPosition_; //!< Holds positional data. Is fetched only once.
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H