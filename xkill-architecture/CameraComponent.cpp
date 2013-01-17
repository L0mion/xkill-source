#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/EventManager.h>
#include "Camera.h"

#include <DirectXMath.h>
#include <windows.h>

ATTRIBUTES_DECLARE_ALL;


void updateProj(int index)
	{
		ZeroMemory(&projection_, sizeof(projection_));

		projection_(0, 0) = 1/(aspect_*(tan(fov_/2)));
		projection_(1, 1) = 1/(tan(fov_/2));
		projection_(2, 2) = zFar_/(zFar_ - zNear_);
		projection_(2, 3) = 1.0f;
		projection_(3, 2) = (-zNear_ * zFar_)/(zFar_ - zNear_);
	}

void yaw(const float angle, Attribute_Camera* camera)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->look); 
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->up);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->right);

	
	float cosAngle = cos((angle)/2);
	float sinAngle = sin((angle)/2);
	
	//Create a Quaternian that describes the rotation.
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(0.0f*sinAngle, 1.0f*sinAngle, 0.0f*sinAngle, cosAngle);
	DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
	vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);
	
	//Rotate all vectors that are affected by the transform. 
	vRight	= DirectX::XMVector3Rotate(vRight, vQuaternion);
	vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
	vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

	//Store the results in member variables.
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->right, vRight);
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&camera->up, vUp);
	

void pitch(const float angle, Attribute_Camera* camera)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->up);
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&camera->look); 

	DirectX::XMVECTOR vStraightUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	//Create a Quaternian that describes the rotation.
	float cosAngle = cos((angle)/2);
	float sinAngle = sin((angle)/2);
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(right_.x*sinAngle, right_.y*sinAngle, right_.z*sinAngle, cosAngle);
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


void mouse(const float dX, const float dY, Attribute_Camera* camera)
{
	yaw(dY, camera);
	pitch(dY, camera);
}

void mouse(const float dX, const float dY, Attribute_Camera* camera)
{
	
	yaw(dX);
	pitch(dY);
}


/// Stores everything RenderComponent needs to know to manage multiple Cameras in the world
/** 
\ingroup ATTRIBUTES
*/
struct Attribute_Camera2 : public IAttribute
{
	AttributePointer ptr_spatial;

	Float4x4 mat_view;
	Float4x4 mat_projection;
	float fov;
	float aspect;
	float zNear;
	float zFar;

	float velocity;				//!< How far the camera will move while a key is pressed.
	DirectX::XMFLOAT3 position;	//!< Where the camera is located.
	DirectX::XMFLOAT3 right;		//!< Always aims to the right of the camera, perpendicular to look.
	DirectX::XMFLOAT3 up;			//!< Always aims up from the camera, perpendicular to look.
	DirectX::XMFLOAT3 look;		//!< The direction in which the camera is aimed. 

	DirectX::XMFLOAT4X4 view;			//!< The view matrix. Used to transform objects to view space.
	DirectX::XMFLOAT4X4 projection;	//!< The projection matrix. Defines the camera's frustum.

	Attribute_Camera2()
	{
		aspect = 0.0f;
		fov = 0.785f; 
		zFar = 40.0f;
		zNear = 0.01f;

		velocity			= 0.01f;

		position	= DirectX::XMFLOAT3(0.0f, 0.0f, -50.0f);
		right		= DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		up			= DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		look		= DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	}
	~Attribute_Camera2()
	{
	}

	//! Moves the camera depending on input from the mouse.
	/*!
	\param dX Mouse delta movement along the x-axis.
	\param dY Mouse delta movement along the y-axis.
	\sa yaw
	\sa pitch
	*/
	void mouse(const float dX, const float dY)
	{
		yaw(dX);
		pitch(dY);
	}

	void keyboard(std::vector<bool>* keys)
	{
		if(keys->at(VK_A))
			strafe(-velocity);
		if(keys->at(VK_D))
			strafe(velocity);
		if(keys->at(VK_S))
			walk(-velocity);
		if(keys->at(VK_W))
			walk(velocity);
	}

	void updateView()
	{
		DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look); 
		DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up);
		DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&right);

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

	void updateProj()
	{
		ZeroMemory(&projection, sizeof(projection));

		projection(0, 0) = 1/(aspect*(tan(fov/2)));
		projection(1, 1) = 1/(tan(fov/2));
		projection(2, 2) = zFar/(zFar - zNear);
		projection(2, 3) = 1.0f;
		projection(3, 2) = (-zNear * zFar)/(zFar - zNear);
	}

	void strafe(const float velocity)
	{
		position.x += right.x * velocity;
		position.y += right.y * velocity;
		position.z += right.z * velocity;
	}

	void walk(const float velocity)
	{
		position.x += look.x * velocity;
		position.y += look.y * velocity;
		position.z += look.z * velocity;
	}

	void pitch(const float angle)
	{

		//Load vectors in to XMVECTORs to utilize SIMD.
		DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look); 
		DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up);

		DirectX::XMVECTOR vStraightUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		//Create a Quaternian that describes the rotation.
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
			DirectX::XMStoreFloat3(&up, vUp);
			DirectX::XMStoreFloat3(&look, vLook);
		}
	}

	void yaw(const float angle)
	{
		//Load vectors in to XMVECTORs to utilize SIMD.
		DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look); 
		DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up);
		DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&right);


		float cosAngle = cos((angle)/2);
		float sinAngle = sin((angle)/2);

		//Create a Quaternian that describes the rotation.
		DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(0.0f*sinAngle, 1.0f*sinAngle, 0.0f*sinAngle, cosAngle);
		DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
		vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);

		//Rotate all vectors that are affected by the transform. 
		vRight	= DirectX::XMVector3Rotate(vRight, vQuaternion);
		vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
		vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

		//Store the results in member variables.
		DirectX::XMStoreFloat3(&right, vRight);
		DirectX::XMStoreFloat3(&up, vUp);
		DirectX::XMStoreFloat3(&look, vLook);
	}

	void setPosition(float position[3])
	{
		this->position.x = position[0] + look.x*0.1f;
		this->position.y = position[1] + look.y*0.1f;
		this->position.z = position[2] + look.z*0.1f;
	}

	void setAspectRatio(float aspectRatio)
	{
		aspect = aspectRatio;
	}

	DirectX::XMFLOAT3 getPosition()
	{
		return position;
	}
	DirectX::XMFLOAT4X4 getView()
	{
		return view;
	}
	DirectX::XMFLOAT4X4 getProjection()
	{
		return projection;
	}
};

CameraComponent::CameraComponent()
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);

	cameraAttributes_	= nullptr;
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::init()
{
	// fetch attributes
	cameraAttributes_ = GET_ATTRIBUTES(camera);
	inputAttributes_ = GET_ATTRIBUTES(input);

	ATTRIBUTES_INIT_ALL;

	// fetch aspect ratio
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);
	float aspectRatio = windowResolution.getAspectRatio();
	Attribute_Camera* cameraAttribute;
	if(cameraAttributes_->size()==2)
	{
		aspectRatio *= 2;
	}
	for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
	{	
		cameraAttribute = &cameraAttributes_->at(i);
		cameraAttribute->aspect = aspectRatio;
		cameras_.push_back(Camera(cameraAttribute->aspect,cameraAttribute->fov,cameraAttribute->zFar,cameraAttribute->zNear));
	}

	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		cameras_[i].updateProj();
		cameraAttributes_->at(i).mat_projection.copy((float*)&cameras_[i].getProjection());
	}

	return true;
}

void CameraComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize((Event_WindowResize*)e);
		break;
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
	while(itrInput.hasNext())
	{
		Attribute_Input* input = itrInput.getNext();
		Entity* entity = itrInput.owner();
		if(entity->hasAttribute(ATTRIBUTE_CAMERA))
		{
			std::vector<Attribute_Camera*> cameras = itrCamera.getMultiple(entity->getAttributes(ATTRIBUTE_CAMERA));
			for(int i=0; i<cameras.size(); i++)
			{
				Attribute_Camera* camera = cameras.at(i);
				camera->yaw(input.rotation.x);
				camera->pitch(input.rotation.y);

				input->rotation.x = 0.0f;
				input->rotation.y = 0.0f;
			}
		}
	}

	// Recalculate view
	while(itrCamera.hasNext())
	{
		Attribute_Camera* camera = itrCamera.getNext();
		Attribute_Spatial* spatial = itrSpatial.at(camera->ptr_spatial);
		Attribute_Position* position = itrPosition.at(spatial->ptr_position);

		camera->setPosition((float*)&position->position);
		camera->updateView();
	}
}


void CameraComponent::event_WindowResize(Event_WindowResize* e)
{
	float aspectRatio = e->getAspectRatio();

	// Recalculate view
	while(itrCamera.hasNext())
	{
		Attribute_Camera* camera = itrCamera.getNext();
		Attribute_Spatial* spatial = itrSpatial.at(camera->ptr_spatial);
		Attribute_Position* position = itrPosition.at(spatial->ptr_position);

		camera->setAspectRatio(aspectRatio);
		camera->updateProj();
	}

	

	for(unsigned int i=0; i<cameras_.size(); i++)
	{
		
	}
	// TODO: resize aspect ratios of all cameras 
}
