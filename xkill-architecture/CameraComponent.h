#pragma once

#include<vector>

#include <xkill-utilities/IObserver.h>

class Camera;
struct CameraAttribute;
struct InputAttribute;
class Event_MouseMove;
class Event_WindowResize;

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
	//! Initializes CameraCompnent. Returns true.
	bool init();
	//! Methods that will be called on events. 
	void onEvent(Event* e);
	//! Methods that will be called on events. 
	void event_WindowResize(Event_WindowResize* e);
	
	//! Update method that will be called every frame.
	void onUpdate(float delta);

private:
	std::vector<Camera> cameras_;						//!< Holds camera objects.
	std::vector<CameraAttribute>* cameraAttributes_;	//!< Holds camera attributes.
	std::vector<InputAttribute>* inputAttributes_;		//!< Holds input attributes.

	float temp;
};