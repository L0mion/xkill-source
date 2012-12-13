#ifndef XKILL_RENDERER_CBMANAGEMENT_H
#define XKILL_RENDERER_CBMANAGEMENT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "d3dInterface.h"

enum CB_TYPE 
{
	CB_TYPE_INSTANCE,
	CB_TYPE_FRAME,
	CB_TYPE_CAMERA,
	CB_TYPE_OBJECT,
	CB_TYPE_BONE
};

static const unsigned int CB_REGISTER_INSTANCE	= 0;
static const unsigned int CB_REGISTER_FRAME		= 1;
static const unsigned int CB_REGISTER_CAMERA	= 2;
static const unsigned int CB_REGISTER_OBJECT	= 3;
static const unsigned int CB_REGISTER_BONE		= 4;

//static const unsigned int 

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

	//! Updates the constant buffer cbInstance.
	void updateCBInstance(ID3D11DeviceContext*	devcon,
						  const unsigned int	screenWidth,
						  const unsigned int	screenHeight);
	//! Updates the constant buffer cbFrame.
	void updateCBFrame(ID3D11DeviceContext* devcon,
					   unsigned int			numLights);

	void updateCBCamera(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT4X4	 viewMatrix,
						DirectX::XMFLOAT4X4	 viewMatrixInverse,
						DirectX::XMFLOAT4X4	 projectionMatrix,
						DirectX::XMFLOAT4X4	 projectionMatrixInverse,
						DirectX::XMFLOAT3	 eyePosition,
						unsigned int		 viewportTopX,
						unsigned int		 viewportTopY);

	//! Updates the constant buffer cbObject.
	void updateCBObject(ID3D11DeviceContext* devcon,
						DirectX::XMFLOAT4X4 finalMatrix,
						DirectX::XMFLOAT4X4 worldMatrix,
						DirectX::XMFLOAT4X4 worldMatrixInverse);
	
	//! Updates the constant buffer cbBone.
	void updateCBBone(ID3D11DeviceContext* devcon,
					  std::vector<DirectX::XMFLOAT4X4> boneTransforms);

	//!Sets a constant buffer the vertex shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void vsSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	//!Sets a constant buffer the pixel shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void psSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon);
	//!Sets a constant buffer the compute shader stage.
	/*!
	\param cbIndex Identifies which constant buffer to use.
	\param shaderRegister Specifies which register in the shader that will be used.
	\param devcon Pointer to a DirectX Device Context.
	*/
	void csSet(CB_TYPE cbType, unsigned int shaderRegister, ID3D11DeviceContext* devcon);

	//! Initializes CBManagement.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device
	\sa initCBFrame
	\sa initCBInstance
	*/
	HRESULT init(ID3D11Device* device);
private:
	//! Initializes a the buffer cbInstance_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbInstance_
	*/
	HRESULT initCBInstance(ID3D11Device* device);
	//! Initializes a the buffer cbFrame_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbFrame_
	*/
	HRESULT initCBFrame(ID3D11Device* device);

	//! Initializes a the buffer cbCamera_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbCamera_
	*/
	HRESULT initCBCamera(ID3D11Device* device);
	//! Initializes a the buffer cbObject_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbObject_
	*/
	HRESULT initCBObject(ID3D11Device* device);
	//! Initializes a the buffer cbBone_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbBone_
	*/
	HRESULT initCBBone(ID3D11Device* device);

	ID3D11Buffer* cbInstance_;	//!< A constant buffer that will be updated once per instance.
	ID3D11Buffer* cbFrame_;		//!< A constant buffer that will be updated every frame.
	ID3D11Buffer* cbCamera_;	//!< A constant buffer that will be updated for every camera.
	ID3D11Buffer* cbObject_;	//!< A constant buffer that will be updated once per object every frame.
	ID3D11Buffer* cbBone_;		//!< A constant buffer containing bones and will be updated for each animated object.
};

#endif