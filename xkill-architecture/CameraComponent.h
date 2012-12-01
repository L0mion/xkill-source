#pragma once

#include<vector>

#include <xkill-utilities/IObserver.h>

class Camera;
struct CameraAttribute;
struct InputAttribute;
class Event_MouseMove;

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
	void init();
	//! Methods that will be called on events. 
	void onEvent(Event* e);
	
	//! Update method that will be called every frame.
	void onUpdate(float delta);

	//! Handles Event_MouseMove events.
	void event_MouseMove(Event_MouseMove* e);

private:
	std::vector<Camera> cameras_;						//!< Holds camera objects.
	std::vector<CameraAttribute>* cameraAttributes_;	//!< Holds camera attributes.
	std::vector<InputAttribute>* inputAttributes_;		//!< Holds input attributes.
};