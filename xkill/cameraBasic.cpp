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
	this->aspect	= aspect;
	this->fov		= fov;
	this->zFar		= zFar;
	this->zNear		= zNear;

	this->screenWidth	= screenWidth;
	this->screenHeight	= screenHeight;

	position	= VecF3(0.0f, 0.0f, -150.0f);
	right		= VecF3(1.0f, 0.0f, 0.0f);
	up			= VecF3(0.0f, 1.0f, 0.0f);
	look		= VecF3(0.0f, 0.0f, 1.0f);
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
	look.normalize();

	up = look.cross(right);
	up.normalize();

	right = up.cross(look);
	right.normalize();

	float x = -position.dot(right);
	float y = -position.dot(up);
	float z = -position.dot(look);

	view.m[0][0] = right.x;
	view.m[1][0] = right.y;
	view.m[2][0] = right.z;
	view.m[3][0] = x;

	view.m[0][1] = up.x;
	view.m[1][1] = up.y;
	view.m[2][1] = up.z;
	view.m[3][1] = y;

	view.m[0][2] = look.x;
	view.m[1][2] = look.y;
	view.m[2][2] = look.z;
	view.m[3][2] = z;

	view.m[0][3] = 0.0f;
	view.m[1][3] = 0.0f;
	view.m[2][3] = 0.0f;
	view.m[3][3] = 1.0f;
}

void CameraBasic::updateProj()
{
	MatF4 perspective;
	ZeroMemory(&perspective, sizeof(MatF4));

	perspective.m[0][0] = 1/(aspect*(tan(fov/2)));
	perspective.m[1][1] = 1/(tan(fov/2));
	perspective.m[2][2] = zFar/(zFar - zNear);
	perspective.m[2][3] = 1.0f;
	perspective.m[3][2] = (-zNear * zFar)/(zFar - zNear);

	projection = perspective;
}

void CameraBasic::strafe(const float velocity)
{
	position += right*velocity;
}

void CameraBasic::walk(const float velocity)
{
	position += look*velocity;
}

void CameraBasic::pitch(const float angle)
{
	up.rotate(angle, right);
	look.rotate(angle, right);
}

void CameraBasic::yaw(const float angle)
{
	VecF3 axis = VecF3(0.0f, 1.0f, 0.0f);
	right.rotate(angle, axis);
	up.rotate(angle, axis);
	look.rotate(angle, axis);
}

VecF3 CameraBasic::getPosition()
{
	return position;
}
MatF4 CameraBasic::getView()
{
	return view;
}
MatF4 CameraBasic::getProjection()
{
	return projection;
}