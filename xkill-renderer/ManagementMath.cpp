#include <DirectXMath.h>

#include <xkill-utilities/AttributeType.h>

#include "ManagementMath.h"

ManagementMath::ManagementMath()
{
	//Do nothing.
}
ManagementMath::~ManagementMath()
{
	//Do nothing.
}

DirectX::XMFLOAT4X4 ManagementMath::calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix)
{
	DirectX::CXMMATRIX	cxmMatrix		= DirectX::XMLoadFloat4x4(&matrix);
	DirectX::XMVECTOR	vDeterminant	= DirectX::XMMatrixDeterminant(cxmMatrix);
	DirectX::XMMATRIX	xmMatrixInverse = DirectX::XMMatrixInverse(&vDeterminant, cxmMatrix);
	
	DirectX::XMFLOAT4X4 matrixInverse;
	DirectX::XMStoreFloat4x4(&matrixInverse, xmMatrixInverse);
	
	return matrixInverse;
}
DirectX::XMFLOAT4X4 ManagementMath::calculateWorldMatrix(
	SpatialAttribute* spatialAttribute, 
	PositionAttribute* positionAttribute)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(positionAttribute->position.x,
																 positionAttribute->position.y,
																 positionAttribute->position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(spatialAttribute->scale.x,
														 spatialAttribute->scale.y,
														 spatialAttribute->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(spatialAttribute->rotation.x,
													spatialAttribute->rotation.y,
													spatialAttribute->rotation.z,
													spatialAttribute->rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, mWorldMatrix);

	return worldMatrix;
}
DirectX::XMFLOAT4X4 ManagementMath::calculateFinalMatrix(
	DirectX::XMFLOAT4X4 worldMatrix, 
	DirectX::XMFLOAT4X4 viewMatrix, 
	DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::CXMMATRIX mWorld		= DirectX::XMLoadFloat4x4(&worldMatrix);

	DirectX::XMMATRIX mFinalMatrix = mWorld * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
}