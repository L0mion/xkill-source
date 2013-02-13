#include "CameraComponent.h"

#include <xkill-utilities/Util.h>

#include <DirectXMath.h>

ATTRIBUTES_DECLARE_ALL;

void updateView(AttributePtr<Attribute_Camera> camera)
{
	// Fetch attributes from camera
	Float3& playerPosition		= camera->ptr_spatial->ptr_position->position;
	
	DirectX::XMFLOAT4X4& view	= *(DirectX::XMFLOAT4X4*)&camera->mat_view;
	DirectX::XMFLOAT3& look		= *(DirectX::XMFLOAT3*)&camera->look; 
	
	DirectX::XMFLOAT3& up		= *(DirectX::XMFLOAT3*)&camera->up;
	DirectX::XMFLOAT3& right	= *(DirectX::XMFLOAT3*)&camera->right;

	DirectX::XMVECTOR& vLook	= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->look); 
	DirectX::XMVECTOR& vUp		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->up);
	DirectX::XMVECTOR& vRight	= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->right);

	// Add offset to camera position
	Float3 position = playerPosition;
	//position.x += look.x*0.1f;
	//position.y += look.y*0.1f;
	//position.z += look.z*0.1f;

	float& zFar			= camera->zFar;
	float& zNear		= camera->zNear;
	float& aspectRatio	= camera->aspectRatio;
	float& fieldOfView	= camera->fieldOfView;


	//
	// Do calculation as normal
	//

	vLook = DirectX::XMVector3Normalize(vLook);

	vUp = DirectX::XMVector3Cross(vLook, vRight);
	vUp = DirectX::XMVector3Normalize(vUp);

	vRight = DirectX::XMVector3Cross(vUp, vLook);
	vRight = DirectX::XMVector3Normalize(vRight);

	DirectX::XMVECTOR vPosition = DirectX::XMVectorSet( position.x*-1.0f,
		position.y*-1.0f,
		position.z*-1.0f,
		1.0f);

	DirectX::XMVECTOR vResult;
	DirectX::XMFLOAT3 fResult;
	vResult = DirectX::XMVector3Dot(vPosition, vRight);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float x = fResult.x;
	vResult = DirectX::XMVector3Dot(vPosition, vUp);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float y = fResult.y;
	vResult = DirectX::XMVector3Dot(vPosition, vLook);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float z = fResult.z;

	DirectX::XMStoreFloat3(&right, vRight);
	DirectX::XMStoreFloat3(&up, vUp);
	DirectX::XMStoreFloat3(&look, vLook);

	view(0, 0) = right.x;
	view(1, 0) = right.y;
	view(2, 0) = right.z;
	view(3, 0) = x;

	view(0, 1) = up.x;
	view(1, 1) = up.y;
	view(2, 1) = up.z;
	view(3, 1) = y;

	view(0, 2) = look.x;
	view(1, 2) = look.y;
	view(2, 2) = look.z;
	view(3, 2) = z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;
}

void updateProj(AttributePtr<Attribute_Camera> camera)
{
	DirectX::XMFLOAT4X4& projection = *(DirectX::XMFLOAT4X4*)&camera->mat_projection;
	float& zFar			= camera->zFar;
	float& zNear		= camera->zNear;
	float& aspectRatio	= camera->aspectRatio;
	float& fieldOfView	= camera->fieldOfView;

	memset(&projection, 0, sizeof(projection));

	projection(0, 0) = 1/(aspectRatio*(tan(fieldOfView/2)));
	projection(1, 1) = 1/(tan(fieldOfView/2));
	projection(2, 2) = zFar/(zFar - zNear);
	projection(2, 3) = 1.0f;
	projection(3, 2) = (-zNear * zFar)/(zFar - zNear);
}

void yaw(const float angle, AttributePtr<Attribute_Camera> camera)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->look); 
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->up);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->right);

	float cosAngle = cos((angle)/2);
	float sinAngle = sin((angle)/2);

	//Create a Quaternion that describes the rotation.
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(0.0f*sinAngle, 1.0f*sinAngle, 0.0f*sinAngle, cosAngle);
	DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
	vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);

	//Rotate all vectors that are affected by the transform. 
	vRight	= DirectX::XMVector3Rotate(vRight, vQuaternion);
	vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
	vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

	//Store the results in member variables.
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->right,	vRight);
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->up,		vUp);
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->look,	vLook);
}


void pitch(const float angle, AttributePtr<Attribute_Camera> camera)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vUp			= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->up);
	DirectX::XMVECTOR vLook			= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->look); 
	DirectX::XMVECTOR vStraightUp	= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Float3& right					= camera->right;

	//Create a Quaternion that describes the rotation.
	float cosAngle = cos((angle)/2);
	float sinAngle = sin((angle)/2);
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(right.x*sinAngle, right.y*sinAngle, right.z*sinAngle, cosAngle);
	DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
	vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);  

	//Rotate all vectors that are affected by the transform. 
	vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
	vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

	//Check the angle between the camera's up-vector and "straight up".
	DirectX::XMFLOAT3 fResult;
	DirectX::XMVECTOR vResult = DirectX::XMVector3AngleBetweenNormals(vUp, vStraightUp);
	DirectX::XMStoreFloat3(&fResult, vResult);

	//Store the results in member variables if the the angle between the camera's up-vector
	//and "straight up" is less than pi/2.
	if(fResult.x < DirectX::XM_PI/2 && fResult.x > -DirectX::XM_PI/2)
	{
		DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->up, vUp);
		DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->look, vLook);
	}
}

void mouse(const float dX, const float dY, AttributePtr<Attribute_Camera> camera)
{
	yaw(dY, camera);
	pitch(dY, camera);
}

void updateAspectRatio()
{
	// Recalculate view and 
	// resize aspect ratios of all cameras 
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		AttributePtr<Attribute_Camera> ptr_camera = ptr_splitScreen->ptr_camera;
		AttributePtr<Attribute_Spatial> ptr_spatial = ptr_camera->ptr_spatial;
		AttributePtr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;

		ptr_camera->aspectRatio = ptr_splitScreen->getAspectRatio();
		updateProj(ptr_camera);
		updateView(ptr_camera);
	}
}

CameraComponent::CameraComponent()
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_SPLITSCREEN_CHANGED);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	ATTRIBUTES_INIT_ALL;
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::init()
{
	return true;
}

void CameraComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_SPLITSCREEN_CHANGED:
		updateAspectRatio();
		break;
	case EVENT_START_DEATHMATCH:
		init();
	default:
		break;
	}
}

void CameraComponent::onUpdate(float delta)
{
	//Event_GetWindowResolution windowResolution;
	//SEND_EVENT(&windowResolution);
	//// Calculate split screen attribute
	//float aspectRatio = windowResolution.getAspectRatio();

	// Update rotation
	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

		AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
		AttributePtr<Attribute_Input> ptr_input = ptr_player->ptr_input;
		AttributePtr<Attribute_Camera> ptr_camera = ptr_player->ptr_camera;
		if(ptr_health->health <= 0)
		{
			//camera->aspectRatio += delta*100;

			//updateProj(camera);
		}
		else
		{
			yaw(ptr_input->rotation.x, ptr_camera);
			pitch(ptr_input->rotation.y, ptr_camera);
		}
		
		//Entity* entity = itrInput.owner();
		//if(entity->hasAttribute(ATTRIBUTE_CAMERA))
		//{
			//std::vector<Attribute_Camera*> cameras = itrCamera.getMultiple(entity->getAttributes(ATTRIBUTE_CAMERA));
			//for(int i=0; i<(int)cameras.size(); i++)
			//{
				//Attribute_Camera* camera = itrCamera.at(player->ptr_camera);//cameras.at(i);
				//yaw(input->rotation.x, camera);
				//pitch(input->rotation.y, camera);
			//}
		//}
	}

	// Recalculate view
	while(itrCamera.hasNext())
	{
		AttributePtr<Attribute_Camera> ptr_camera = itrCamera.getNext();
		AttributePtr<Attribute_Spatial> ptr_spatial = ptr_camera->ptr_spatial;
		updateView(ptr_camera);
		updateProj(ptr_camera);
	}
}