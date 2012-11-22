#include <Windows.h>

#include "CameraBasic.h"

CameraBasic::CameraBasic(
	float aspect, 
	float fov, 
	float zFar, 
	float zNear, 
	unsigned int screenWidth, 
	unsigned int screenHeight)
{
	this->aspect_	= aspect;
	this->fov_		= fov;
	this->zFar_		= zFar;
	this->zNear_		= zNear;

	this->screenWidth_	= screenWidth;
	this->screenHeight_	= screenHeight;

	position_	= VecF3(0.0f, 0.0f, -150.0f);
	right_		= VecF3(1.0f, 0.0f, 0.0f);
	up_			= VecF3(0.0f, 1.0f, 0.0f);
	look_		= VecF3(0.0f, 0.0f, 1.0f);
}

CameraBasic::~CameraBasic()
{
}

void CameraBasic::mouse(const float dX, const float dY)
{
	yaw(dX);
	pitch(dY);
}

void CameraBasic::keyboard()
{
	//switch(key)
	//{
	//case KEY_W:
	// walk(STEP_SCALE);
	// break;
	//case KEY_A:
	// strafe(-STEP_SCALE);
	// break;
	//case KEY_S:
	// walk(-STEP_SCALE);
	// break;
	//case KEY_D:
	// //strafe(STEP_SCALE);
	// break;
	//default:
	// break;
	//}
}

void CameraBasic::updateView()
{
	look_.normalize();

	up_ = look_.cross(right_);
	up_.normalize();

	right_ = up_.cross(look_);
	right_.normalize();

	float x = -position_.dot(right_);
	float y = -position_.dot(up_);
	float z = -position_.dot(look_);

	view_.m[0][0] = right_.x;
	view_.m[1][0] = right_.y;
	view_.m[2][0] = right_.z;
	view_.m[3][0] = x;

	view_.m[0][1] = up_.x;
	view_.m[1][1] = up_.y;
	view_.m[2][1] = up_.z;
	view_.m[3][1] = y;

	view_.m[0][2] = look_.x;
	view_.m[1][2] = look_.y;
	view_.m[2][2] = look_.z;
	view_.m[3][2] = z;

	view_.m[0][3] = 0.0f;
	view_.m[1][3] = 0.0f;
	view_.m[2][3] = 0.0f;
	view_.m[3][3] = 1.0f;
}

void CameraBasic::updateProj()
{
	MatF4 perspective;
	ZeroMemory(&perspective, sizeof(MatF4));

	perspective.m[0][0] = 1/(aspect_*(tan(fov_/2)));
	perspective.m[1][1] = 1/(tan(fov_/2));
	perspective.m[2][2] = zFar_/(zFar_ - zNear_);
	perspective.m[2][3] = 1.0f;
	perspective.m[3][2] = (-zNear_ * zFar_)/(zFar_ - zNear_);

	projection_ = perspective;
}

void CameraBasic::strafe(const float velocity)
{
	position_ += right_*velocity;
}

void CameraBasic::walk(const float velocity)
{
	position_ += look_*velocity;
}

void CameraBasic::pitch(const float angle)
{
	up_.rotate(angle, right_);
	look_.rotate(angle, right_);
}

void CameraBasic::yaw(const float angle)
{
	VecF3 axis = VecF3(0.0f, 1.0f, 0.0f);
	right_.rotate(angle, axis);
	up_.rotate(angle, axis);
	look_.rotate(angle, axis);
}

VecF3 CameraBasic::getPosition()
{
	return position_;
}
MatF4 CameraBasic::getView()
{
	return view_;
}
MatF4 CameraBasic::getProjection()
{
	return projection_;
}