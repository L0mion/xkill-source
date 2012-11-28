#pragma once

#include<vector>

#include <xkill-utilities/IObserver.h>

class Camera;
struct CameraAttribute;
struct InputAttribute;
/// Component to process PhysicsAttribute
/** 
\ingroup components
*/

class CameraComponent : public IObserver
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	void init(std::vector<CameraAttribute>* cameraAttributes,
			std::vector<InputAttribute>* inputAttributes,
			float aspectRatio);

	void onEvent(Event* e);
	
	void onUpdate(float delta);

private:
	std::vector<Camera> cameras_;
	std::vector<CameraAttribute>* cameraAttributes_;
	std::vector<InputAttribute>* inputAttributes_;
};