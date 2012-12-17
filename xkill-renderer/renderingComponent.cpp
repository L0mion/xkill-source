#include <iostream>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/TexDesc.h>

#include "d3dDebug.h"
#include "CBManagement.h"
#include "LightManagement.h"
#include "ModelManagement.h"
#include "TexManagement.h"
#include "ModelD3D.h"
#include "VB.h"
#include "IB.h"
#include "SubsetD3D.h"

#include "M3DLoader.h"
#include "AnimatedMesh.h"
#include "SkinnedData.h"
#include "Renderer.h"
#include "renderingUtilities.h"
#include "renderingComponent.h"

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	//Subscribe to all the events RenderingComponent will need during execution.
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_POST_DESC_TEX);

	windowHandle_	= windowHandle;
	renderer_		= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	SAFE_DELETE(renderer_);
}

void RenderingComponent::reset()
{
	SAFE_RESET(renderer_);
}
HRESULT RenderingComponent::init()
{
	HRESULT hr = S_OK;
	
	renderer_ = new Renderer(windowHandle_);
	hr = renderer_->init();

	return hr;
}

void RenderingComponent::onUpdate(float delta)
{
	renderer_->render(delta);
}
void RenderingComponent::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize((Event_WindowResize*)e);
		break;
	case EVENT_POST_DESC_TEX:
		event_PostDescTex((Event_PostDescTex*)e);
		break;
	default:
		break;
	}
}

void RenderingComponent::event_WindowResize( Event_WindowResize* e )
{
	int width = e->width;
	int height = e->height;

	renderer_->resize(width, height);
}
void RenderingComponent::event_PostDescTex(Event_PostDescTex* e)
{
	TexDesc* texDesc = e->texDesc_;

	renderer_->loadTextures(texDesc);

	delete texDesc;
}

/*
void RenderingComponent::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	ID3D11Device*			device = d3dManagement_->getDevice();
	ID3D11DeviceContext*	devcon = d3dManagement_->getDeviceContext();

	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.01f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.01f, 0.0f,
									10.0f, 2.3f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	cbManagement_->vsSet(CB_TYPE_OBJECT, CB_REGISTER_OBJECT, devcon);
	cbManagement_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
	animatedMesh_->update(0.002f);
	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);

	cbManagement_->vsSet(CB_TYPE_BONE, CB_REGISTER_BONE, devcon);
	cbManagement_->updateCBBone(devcon, finalTransforms);

	fxManagement_->getAnimationVS()->set(devcon);
	fxManagement_->getAnimationPS()->set(devcon);
	ssManagement_->setPS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);
	rsManagement_->setRS(d3dManagement_->getDeviceContext(), RS_ID_DEFAULT);

	renderGBufferSetRenderTargets();
	d3dManagement_->clearDepthBuffer();

	ID3D11Buffer* vertexBuffer = animatedMesh_->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTexTanSkinned);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(animatedMesh_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	devcon->IASetInputLayout(fxManagement_->getILPosNormTexTanSkinned());
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);

	renderGBufferClean();
}
*/