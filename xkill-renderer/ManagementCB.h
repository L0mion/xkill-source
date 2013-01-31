#ifndef XKILL_RENDERER_CBMANAGEMENT_H
#define XKILL_RENDERER_CBMANAGEMENT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "d3dInterface.h"
#include "TypeFX.h"

enum CB_TYPE 
{
	CB_TYPE_INSTANCE,
	CB_TYPE_FRAME,
	CB_TYPE_CAMERA,
	CB_TYPE_OBJECT,
	CB_TYPE_SUBSET,
	CB_TYPE_BONE,
	CB_TYPE_SPRITE
};

static const unsigned int CB_REGISTER_INSTANCE	= 0;
static const unsigned int CB_REGISTER_FRAME		= 1;
static const unsigned int CB_REGISTER_CAMERA	= 2;
static const unsigned int CB_REGISTER_OBJECT	= 3;
static const unsigned int CB_REGISTER_SUBSET	= 4;
static const unsigned int CB_REGISTER_BONE		= 5;
static const unsigned int CB_REGISTER_SPRITE	= 6;

//static const unsigned int 

//! Class for maintaining constant buffers.
/*!
\ingroup xkill-renderer
*/
class ManagementCB : public D3DInterface
{
public:
	ManagementCB();		//!< Initializes CBManagement to its default state.
	~ManagementCB();	//!< Releases all memory and resets CBManagement to its default state.
	void reset();		//!< Releases all memory and resets CBManagement to its default state.

	void updateCBInstance(
		ID3D11DeviceContext*	devcon,
		 const unsigned int		screenWidth,
		 const unsigned int		screenHeight);		//!< Updates the constant buffer cbInstance.
	void updateCBFrame(
		ID3D11DeviceContext*	devcon,
		unsigned int			numLightsDir,
		unsigned int			numLightsPoint,
		unsigned int			numLightsSpot);		//!< Updates the constant buffer cbFrame.
	void updateCBCamera(
		ID3D11DeviceContext* devcon,
		DirectX::XMFLOAT4X4	 viewMatrix,
		DirectX::XMFLOAT4X4	 viewMatrixInverse,
		DirectX::XMFLOAT4X4	 projectionMatrix,
		DirectX::XMFLOAT4X4	 projectionMatrixInverse,
		DirectX::XMFLOAT3	 eyePosition,
		unsigned int		 viewportTopX,
		unsigned int		 viewportTopY,
		float				zNear,
		float				zFar);	 //!< Updates the constant buffer cbCamera.
	void updateCBObject(
		ID3D11DeviceContext* devcon,
		DirectX::XMFLOAT4X4 finalMatrix,
		DirectX::XMFLOAT4X4 worldMatrix,
		DirectX::XMFLOAT4X4 worldMatrixInverse); //!< Updates the constant buffer cbObject.
	void updateCBBone(
		ID3D11DeviceContext* devcon,
		std::vector<DirectX::XMFLOAT4X4> boneTransforms); //!< Updates the constant buffer cbBone.
	void updateCBSubset(
		ID3D11DeviceContext* devcon,
		DirectX::XMFLOAT3	specularTerm,
		float				specularPower); //!< Updates the constant buffer cbSubset.
	void updateCBSprite(
		ID3D11DeviceContext* devcon,
		DirectX::XMFLOAT4X4 transformMatrix); //!< Updates the constant buffer cbSprite.
		
	void setCB(
		CB_TYPE					cbType, 
		TypeFX					shaderStage, 
		unsigned int			shaderRegister, 
		ID3D11DeviceContext*	devcon); //!< Sets specified constant buffer.

	HRESULT init(ID3D11Device* device); //! Initializes CBManagement.
private:
	HRESULT initCBInstance(ID3D11Device* device);	//!< Initializes the buffer cbInstance_.
	HRESULT initCBFrame(ID3D11Device* device);		//!< Initializes the buffer cbFrame_.
	HRESULT initCBCamera(ID3D11Device* device);		//!< Initializes the buffer cbCamera_.
	HRESULT initCBObject(ID3D11Device* device);		//!< Initializes the buffer cbObject_.
	HRESULT initCBBone(ID3D11Device* device);		//!< Initializes the buffer cbBone_.
	HRESULT initCBSubset(ID3D11Device* device);		//!< Initializes the buffer cbSubset_.

	//! Initializes a the buffer cbSprite_.
	/*!
	\return Any error encountered.
	\param device Pointer to DirectX Device.
	\sa cbSprite_
	*/
	HRESULT initCBSprite(ID3D11Device* device);

	ID3D11Buffer* cbInstance_;	//!< A constant buffer that will be updated once per instance.
	ID3D11Buffer* cbFrame_;		//!< A constant buffer that will be updated every frame.
	ID3D11Buffer* cbCamera_;	//!< A constant buffer that will be updated for every camera.
	ID3D11Buffer* cbObject_;	//!< A constant buffer that will be updated once per object every frame.
	ID3D11Buffer* cbSubset_;	//!< A constant buffer that will be updated once per subset.
	ID3D11Buffer* cbBone_;		//!< A constant buffer containing bones and will be updated for each animated object.
	ID3D11Buffer* cbSprite_;	//!< A constant buffer containing transformation matrix for a sprite.
};

#endif