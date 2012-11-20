#ifndef XKILL_RENDERER_CAMERABASIC_H
#define XKILL_RENDERER_CAMERABASIC_H

#include "mathBasic.h"
#include "dllRenderer.h"

class DLL CameraBasic
{
public:
	CameraBasic(
		float aspect, 
		float fov, 
		float zFar, 
		float zNear,
		unsigned int screenWidth,
		unsigned int screenHeight);
	virtual ~CameraBasic();

	virtual void mouse(const float dX, const float dY);
	virtual void keyboard();

	virtual void updateView();
	virtual void updateProj();

	virtual void strafe(const float velocity);
	virtual void walk(const float velocity);
	virtual void pitch(const float angle);
	virtual void yaw(const float angle);

	VecF3 getPosition();
	MatF4 getView();
	MatF4 getProjection();
private:
	float fov;
	float aspect;
	float zNear;
	float zFar;
	unsigned int screenWidth;
	unsigned int screenHeight;

	float STEP_SCALE;
	VecF3 position;
	VecF3 right;
	VecF3 up;
	VecF3 look;
	
	MatF4 view;
	MatF4 projection;
};

#endif //XKILL_RENDERER_CAMERABASIC_H