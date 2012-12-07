#include <Windows.h>

#include "Camera.h"

Camera::Camera(
		float aspect, 
		float fov, 
		float zFar, 
		float zNear)
{
	this->aspect_	= aspect;
	this->fov_		= fov;
	this->zFar_		= zFar;
	this->zNear_		= zNear;

	velocity_			= 0.01f;

	position_	= DirectX::XMFLOAT3(0.0f, 0.0f, -50.0f);
	right_		= DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	up_			= DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	look_		= DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{
}

void Camera::mouse(const float dX, const float dY)
{
	yaw(dX);
	pitch(dY);
}

void Camera::keyboard(std::vector<bool>* keys)
{
	if(keys->at(VK_A))
		strafe(-velocity_);
	if(keys->at(VK_D))
		strafe(velocity_);
	if(keys->at(VK_S))
		walk(-velocity_);
	if(keys->at(VK_W))
		walk(velocity_);
}

void Camera::updateView()
{
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look_); 
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up_);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&right_);

	vLook = DirectX::XMVector3Normalize(vLook);

	vUp = DirectX::XMVector3Cross(vLook, vRight);
	vUp = DirectX::XMVector3Normalize(vUp);

	vRight = DirectX::XMVector3Cross(vUp, vLook);
	vRight = DirectX::XMVector3Normalize(vRight);
	
	DirectX::XMVECTOR vPosition = DirectX::XMVectorSet( position_.x*-1.0f,
														position_.y*-1.0f,
														position_.z*-1.0f,
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

	DirectX::XMStoreFloat3(&right_, vRight);
	DirectX::XMStoreFloat3(&up_, vUp);
	DirectX::XMStoreFloat3(&look_, vLook);

	view_(0, 0) = right_.x;
	view_(1, 0) = right_.y;
	view_(2, 0) = right_.z;
	view_(3, 0) = x;

	view_(0, 1) = up_.x;
	view_(1, 1) = up_.y;
	view_(2, 1) = up_.z;
	view_(3, 1) = y;

	view_(0, 2) = look_.x;
	view_(1, 2) = look_.y;
	view_(2, 2) = look_.z;
	view_(3, 2) = z;

	view_(0, 3) = 0.0f;
	view_(1, 3) = 0.0f;
	view_(2, 3) = 0.0f;
	view_(3, 3) = 1.0f;
}

void Camera::updateProj()
{
	ZeroMemory(&projection_, sizeof(projection_));

	projection_(0, 0) = 1/(aspect_*(tan(fov_/2)));
	projection_(1, 1) = 1/(tan(fov_/2));
	projection_(2, 2) = zFar_/(zFar_ - zNear_);
	projection_(2, 3) = 1.0f;
	projection_(3, 2) = (-zNear_ * zFar_)/(zFar_ - zNear_);
}

void Camera::strafe(const float velocity)
{
	position_.x += right_.x * velocity;
	position_.y += right_.y * velocity;
	position_.z += right_.z * velocity;
}

void Camera::walk(const float velocity)
{
	position_.x += look_.x * velocity;
	position_.y += look_.y * velocity;
	position_.z += look_.z * velocity;
}

void Camera::pitch(const float angle)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look_); 
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up_);

	DirectX::XMVECTOR vLimitUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR vLimitDown = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR vResult;
	DirectX::XMFLOAT3 result;
	
	//Check that the camera does not tip over. 
	vResult = DirectX::XMVector3AngleBetweenVectors(vLook, vLimitUp);
	DirectX::XMStoreFloat3(&result, vResult);
	float angleUp = result.x;
	vResult = DirectX::XMVector3AngleBetweenVectors(vLook, vLimitDown);
	DirectX::XMStoreFloat3(&result, vResult);
	float angleDown = result.x;
	bool canRotate = true;
	if(angleUp < 0.1f && angle < 0)
		canRotate = false;
	if(angleDown < 0.1f && angle > 0)
		canRotate = false;
	
	if(canRotate)
	{
		//Create a Quaternian that describes the rotation.
		float cosAngle = cos((angle)/2);
		float sinAngle = sin((angle)/2);
		DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(right_.x*sinAngle, right_.y*sinAngle, right_.z*sinAngle, cosAngle);
		DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
		vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);

		//Rotate all vectors that are affected by the transform. 
		vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
		vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

		//Store the results in member variables.
		DirectX::XMStoreFloat3(&up_, vUp);
		DirectX::XMStoreFloat3(&look_, vLook);
	}
}

void Camera::yaw(const float angle)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook		= DirectX::XMLoadFloat3(&look_); 
	DirectX::XMVECTOR vUp		= DirectX::XMLoadFloat3(&up_);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&right_);

	
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
	DirectX::XMStoreFloat3(&right_, vRight);
	DirectX::XMStoreFloat3(&up_, vUp);
	DirectX::XMStoreFloat3(&look_, vLook);
}

void Camera::setPosition(float position[3])
{
	position_.x = position[0] + look_.x*0;
	position_.y = position[1] + look_.y*0;
	position_.z = position[2] + look_.z*0;
}

void Camera::setAspectRatio(float aspectRatio)
{
	aspect_ = aspectRatio;
}

DirectX::XMFLOAT3 Camera::getPosition()
{
	return position_;
}
DirectX::XMFLOAT4X4 Camera::getView()
{
	return view_;
}
DirectX::XMFLOAT4X4 Camera::getProjection()
{
	return projection_;
}