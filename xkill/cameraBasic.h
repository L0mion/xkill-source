#ifndef XKILL_CAMERABASIC_H
#define XKILL_CAMERABASIC_H

#include <xkill-renderer/mathBasic.h>

//! Camera of XKILL.
/*!
Describing the virtual eye through which the 3D-world is viewed.
\ingroup xkill-renderer
*/
class CameraBasic
{
public:
	//! Initializes camera member variables to default values.
	/*!
	\param aspect The window's aspect ratio.
	\param fov The camera's field of view.
	\param zFar The far viewing plane.
	\param zNear The near viewing plane.
	\param screenWidth Width of the window/viewport
	\param screenHeight Height of the window/viewport
	*/
	CameraBasic(
		float aspect, 
		float fov, 
		float zFar, 
		float zNear,
		unsigned int screenWidth,
		unsigned int screenHeight);
	//! Does nothing since the camera does not alocate any memory.
	virtual ~CameraBasic();

	//! Moves the camera depending on input from the mouse.
	/*!
	\param dX Mouse delta movement along the x-axis.
	\param dY Mouse delta movement along the y-axis.
	\sa yaw
	\sa pitch
	*/
	virtual void mouse(const float dX, const float dY);
	//! Moves the camera depending on input from the keyboard.
	/*!
	\sa walk
	\sa strafe
	*/
	virtual void keyboard();

	//! Rebuilds the view matrix.
	virtual void updateView();
	//! Rebuilds the projection matrix.
	virtual void updateProj();

	//! Moves the camera along the right-vector.
	/*!
	\param velocity How fast the camera should move.
	*/
	virtual void strafe(const float velocity);
	//! Moves the camera along the look-vector.
	/*!
	\param velocity How fast the camera should move.
	*/
	virtual void walk(const float velocity);
	//! Rotates the camera around the right-vector.
	/*!
	\param angle How far the camera should rotate.
	*/
	virtual void pitch(const float angle);
	//! Rotates the camera around the up-vector.
	/*!
	\param angle How far the camera should rotate.
	*/
	virtual void yaw(const float angle);

	VecF3 getPosition();
	MatF4 getView();
	MatF4 getProjection();
private:
	float fov;					//!< The cameras field of view.
	float aspect;				//!< Aspect ratio of the window.
	float zNear;				//!< The far viewing plane.
	float zFar;					//!< The near viewing plane.
	unsigned int screenWidth;	//!< screenWidth Width of the window/viewport.
	unsigned int screenHeight;	//!< screenHeight Height of the window/viewport.

	float STEP_SCALE;	//!< How far the camera will move while a key is pressed.
	VecF3 position;		//!< Where the camera is located.
	VecF3 right;		//!< Always aims to the right of the camera, perpendicular to look.
	VecF3 up;			//!< Always aims up from the camera, perpendicular to look.
	VecF3 look;			//!< The direction in which the camera is aimed. 
	
	MatF4 view;			//!< The view matrix. Used to transform objects to view space.
	MatF4 projection;	//!< The projection matrix. Defines the camera's frustum.
};

#endif //XKILL_CAMERABASIC_H