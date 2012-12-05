#ifndef XKILL_RENDERER_SSMANAGEMENT_H
#define XKILL_RENDERER_SSMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

enum SS_ID
{
	SS_ID_DEFAULT
};

//! Class for maintaining sampler states.
/*!
\ingroup xkill-renderer
*/
class SSManagement : public D3DInterface
{
public:
	//! Initializes SSManagement to its default state.
	SSManagement();
	//!Releases all memory and resets SSManagement to its default state.
	virtual ~SSManagement();
	//!Releases all memory and resets SSManagement to its default state.
	virtual void reset();

	//! Sets a sampler state the vertex shader stage.
	/*!
	\param devcon Pointer to a DirectX Device Context.
	\param ssId An enum identifying which state to be set.
	\param shaderRegister An index identifying which register in the shader that will be used. 
	*/
	void setVS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);
	//! Sets a sampler state the pixel shader stage.
	/*!
	\param devcon Pointer to a DirectX Device Context.
	\param ssId An enum identifying which state to be set.
	\param shaderRegister An index identifying which register in the shader that will be used. 
	*/
	void setPS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);
	//! Sets a sampler state the compute shader stage.
	/*!
	\param devcon Pointer to a DirectX Device Context.
	\param ssId An enum identifying which state to be set.
	\param shaderRegister An index identifying which register in the shader that will be used. 
	*/
	void setCS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);
	//! Initializes SSManagement's members.
	/*!
	\return Return any error encountered.
	\param device A pointer to a DirectX Device.
	\sa initSSDefault
	*/
	HRESULT init(ID3D11Device* device);
private:
	//! Initializes the variable ssDefault_.
	/*!
	\return Return any error encountered.
	\param device A pointer to a DirectX Device.
	\sa ssDefault_
	*/
	HRESULT initSSDefault(ID3D11Device* device);

	ID3D11SamplerState* ssDefault_; //!< A sampler state object.
};

#endif //XKILL_RENDERER_SSMANAGEMENT_H