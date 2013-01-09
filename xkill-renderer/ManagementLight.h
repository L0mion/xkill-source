#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>

#include "d3dInterface.h"
#include "LightDesc.h"

struct ID3D11UnorderedAccesView;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
//typedef HRESULT;


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

	void update(ID3D11Device* device, ID3D11DeviceContext* devcon);
	
	HRESULT updateBufferData(ID3D11DeviceContext* devcon); //!< Updates data in the light buffer.
	
	void setLightSRVCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister); //!< Set the compute shader to use lightSRV_
	
	void addLight(LightDesc light, ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Adds a light to LightManagement

	unsigned int getNumLights() const;
private:
	//void createDirectionalLight(ID3D11Device* device);	//!< Fills a LightDesc with data necessary for a directionalLight
	void resizeLights(ID3D11Device* device);		//!< Resizes the vector lights_ and also modifies lightBuffer_ and lightSRV_ accordingly.
	HRESULT initLightBuffer(ID3D11Device* device);	//!< Creates a buffer containg all the lights.
	HRESULT createLightSRV(ID3D11Device* device);	//! Creates a Shader Resource View that is bound to lightBuffer_.
	
	unsigned int maxNumLights_;	//!< Size of the vector lights_. Doubles every time the vector becomes full.
	unsigned int curNumLights_;
	std::vector<LightDesc> lights_;

	std::vector<Attribute_Position>* attributesPosition_; //!< Holds positional data. Is fetched only once.

	//unsigned int				numLights_;		//!< Current number of lights.
	//std::vector<LightDesc>		lights_;		//!< A vector containing all lights.
	ID3D11Buffer*				lightBuffer_;	//!< A D3D11Buffer object.
	ID3D11ShaderResourceView*	lightSRV_;		//!< A D3D11ShaderResourceView that will be bound to lightBuffer_.
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H