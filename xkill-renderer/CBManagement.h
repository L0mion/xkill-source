#ifndef XKILL_RENDERER_CBMANAGEMENT_H
#define XKILL_RENDERER_CBMANAGEMENT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "d3dInterface.h"

static const unsigned int CB_FRAME_INDEX	= 1;
static const unsigned int CB_INSTANCE_INDEX = 2;
static const unsigned int CB_OBJECT_INDEX	= 3;

//! Class for maintaining constant buffers.
/*!
\ingroup xkill-renderer
*/
class CBManagement : public D3DInterface
{
public:
	//! Initializes CBManagement to its default state.
	CBManagement();
	//!Releases all memory and resets CBManagement to its default state.
	~CBManagement();
	//!Releases all memory and resets CBManagement to its default state.
	void reset();
	//! Updates the constant buffer cbFrame.
	void updateCBFrame(ID3D11DeviceContext* devcon,
					   DirectX::XMFLOAT4X4	viewMatrix,
					   DirectX::XMFLOAT4X4	viewMatrixInverse,
					   DirectX::XMFLOAT4X4	projectionMatrix,
					   DirectX::XMFLOAT4X4	projectionMatrixInverse,
					   DirectX::XMFLOAT3	eyePosition,
					   unsigned int			numLights);
	//! Updates the constant buffer cbInstance.
	void updateCBInstance(
		ID3D11DeviceContext*	devcon,
		const unsigned int		screenWidth,
		const unsigned int		screenHeight,
		const unsigned int		tileWidth,
		const unsigned int		tileHeight);

	//! Updates the constant buffer cbObject.
	void updateCBObject(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT4X4 finalMatrix,
						DirectX::XMFLOAT4X4 worldMatrix,
						DirectX::XMFLOAT4X4 worldMatrixInverse);

	//!Sets a constant buffer the vertex shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void vsSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	//!Sets a constant buffer the pixel shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void psSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	//!Sets a constant buffer the compute shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void csSet(unsigned int cbIndex, unsigned int shaderRegister, ID3D11DeviceContext* devcon);

	//! Initializes CBManagement.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device
	\sa initCBFrame
	\sa initCBInstance
	*/
	HRESULT init(ID3D11Device* device);
private:

	//! Initializes a the buffer cbFrame_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbFrame_
	*/
	HRESULT initCBFrame(ID3D11Device* device);
	//! Initializes a the buffer cbInstance_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbInstance_
	*/
	HRESULT initCBInstance(ID3D11Device* device);
	//! Initializes a the buffer cbObject_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbObject_
	*/
	HRESULT initCBObject(ID3D11Device* device);

	ID3D11Buffer* cbFrame_;		//!< A constant buffer that will be updated every frame.
	ID3D11Buffer* cbInstance_;	//!< A constant buffer that will be updated once per instance.
	ID3D11Buffer* cbObject_;	//!< Aconstant buffer that will be updated once per object every frame.
};

#endif