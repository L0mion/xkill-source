#pragma once

#include<vector>

#include <xkill-utilities/IObserver.h>

class Camera;
struct CameraAttribute;
struct InputAttribute;

//! Component to process Cameras
/** 
\ingroup components
*/
class CameraComponent : public IObserver
{
public:
	//! Initializes Camera Component to its default state.
	CameraComponent();
	//! Releases all allocated memory.
	virtual ~CameraComponent();
	//! Initializes CameraCompnent.
	void init(std::vector<CameraAttribute>* cameraAttributes,
			std::vector<InputAttribute>* inputAttributes,
			float aspectRatio);
	//! Methods that will be called on events. 
	void onEvent(Event* e);
	
	//! Update method that will be called every frame.
	void onUpdate(float delta);

private:
	std::vector<Camera> cameras_;						//!< Holds camera objects.
	std::vector<CameraAttribute>* cameraAttributes_;	//!< Holds camera attributes.
	std::vector<InputAttribute>* inputAttributes_;		//!< Holds input attributes.
};