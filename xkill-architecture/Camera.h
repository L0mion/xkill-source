#ifndef XKILL_CAMERA_H
#define XKILL_CAMERA_H

#include <DirectXMath.h>
#include <vector>

#define VK_A 0x41
#define VK_D 0x44
#define VK_S 0x53
#define VK_W 0x57

//! Camera of XKILL.
/*!
Describing the virtual eye through which the 3D-world is viewed.
\ingroup xkill-renderer
*/
class Camera
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
	Camera(
		float aspect, 
		float fov = 0.785f, 
		float zFar = 10.0f, 
		float zNear = 0.01f);
	//! Does nothing since the camera does not alocate any memory.
	virtual ~Camera();

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
	\param keys Vector where each index maps to a key on the keyboard.
	\sa walk
	\sa strafe
	*/
	virtual void keyboard(std::vector<bool>* keys);

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

	void setPosition(float position[3]);
	void setRotationToZero();
	void setAspectRatio(float aspectRatio);

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getProjection();
private:
	float fov_;					//!< The cameras field of view.
	float aspect_;				//!< Aspect ratio of the window.
	float zNear_;				//!< The far viewing plane.
	float zFar_;				//!< The near viewing plane.

	float velocity_;				//!< How far the camera will move while a key is pressed.
	DirectX::XMFLOAT3 position_;	//!< Where the camera is located.
	DirectX::XMFLOAT3 right_;		//!< Always aims to the right of the camera, perpendicular to look.
	DirectX::XMFLOAT3 up_;			//!< Always aims up from the camera, perpendicular to look.
	DirectX::XMFLOAT3 look_;		//!< The direction in which the camera is aimed. 
	
	DirectX::XMFLOAT4X4 view_;			//!< The view matrix. Used to transform objects to view space.
	DirectX::XMFLOAT4X4 projection_;	//!< The projection matrix. Defines the camera's frustum.
};

#endif //XKILL_CAMERABASIC_H