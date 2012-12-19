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
	//! Sets LightManagement to its default state.
	ManagementLight();
	//! Releases all memory and returns LightManagement to its default state.
	~ManagementLight();
	//! Releases all memory and returns LightManagement to its default state.
	virtual void reset();
	//! Initializes LightManagement.
	/*!
	\param device Pointer to DirectX Device
	\sa createDirectionalLight
	\sa createLightBuffer
	\sa createLightSRV
	*/
	HRESULT init(ID3D11Device* device);
	//! Updates data in the light buffer.
	/*!
	This function takes exclusive control of the lightBuffer object, meaning the GPU has no access to it.
	It then uses memcpy to transfer data from the vector lights_ to the buffer lightBuffer_.
	\param device A pointer to a DirectX Devive
	\sa lights_
	\sa lightBuffer_
	\return Any error encountered.
	*/
	HRESULT updateBufferData(ID3D11DeviceContext* devcon);
	//! Set the compute shader to use lightSRV_
	/*!
	\param devcon A pointer to a DirectX Device Context.
	\param shaderRegister An index to a register on the gpu to which the shader resource view is bound.
	\return Any error encountered.
	*/
	void setLightSRVCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	//! Adds a light to LightManagement
	/*!
	\param light A LightDesc object describing a light source.
	\param device A pointer to a DirectX Device.
	\param devcon A pointer to a DirectX Device Context.
	\sa LightDesc
	*/
	void addLight(LightDesc light, ID3D11Device* device, ID3D11DeviceContext* devcon);

	unsigned int getNumLights() const;
private:
	//! Fills a LightDesc with data necessary for a directionalLight
	/*!
	\param device Pointer to a DirectX Device.
	\sa LightDesc
	*/
	void createDirectionalLight(ID3D11Device* device);
	//! Resizes the vector lights_ and also modifies lightBuffer_ and lightSRV_ accordingly.
	/*!
	\param device A pointer to DirectX Device.
	*/
	void resizeLights(ID3D11Device* device);
	//! Creates a buffer containg all the lights.
	/*!
	\param device A pointer to DirectX Device.
	\return Any error encountered.
	*/
	HRESULT createLightBuffer(ID3D11Device* device);
	//! Creates a Shader Resource View that is bound to lightBuffer_.
	/*!
	\param device A pointer to DirectX Device.
	\return Any error encountered.
	*/
	HRESULT createLightSRV(ID3D11Device* device);
	
	unsigned int				maxNumLights_;	//!< Size of the vector lights_. Doubles every time the vector becomes full.
	unsigned int				numLights_;		//!< Current number of lights.
	std::vector<LightDesc>		lights_;		//!< A vector containing all lights.
	ID3D11Buffer*				lightBuffer_;	//!< A D3D11Buffer object.
	ID3D11ShaderResourceView*	lightSRV_;		//!< A D3D11ShaderResourceView that will be bound to lightBuffer_.
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H