#pragma once

#include <xkill-renderer/renderingComponent.h>
#include "SoundComponent.h"
#include "PhysicsComponent.h"
#include "CameraComponent.h"
#include "AttributeManager.h"
#include <vector>


/// Is responsible for updating Components in a certain order
/** 
Some Components such as RenderComponent and PhysicsComponent needs to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order (InputComponent 
should to run before the RenderComponent to hide latency) and this should 
be the responsiblity of the ComponentManager.

\ingroup achitecture
*/

class ComponentManager
{
private:
	RenderingComponent* render_;
	PhysicsComponent	physics_;
	SoundComponent		sound_;
	CameraComponent		camera_;
public:
	ComponentManager()
	{
		
	}
	~ComponentManager()
	{
		if(render_)
			delete render_;
	}

	void init(HWND windowHandle, unsigned int screenWidth, unsigned int screenHeight)
	{
		render_ = new RenderingComponent(windowHandle,screenWidth,screenHeight,800,800,1,
										AttributeManager::getInstance()->renderAttributes.getAllAttributes(),
										AttributeManager::getInstance()->cameraAttributes.getAllAttributes());
		render_->init();
		camera_.init(AttributeManager::getInstance()->cameraAttributes.getAllAttributes(),
					AttributeManager::getInstance()->inputAttributes.getAllAttributes(),
					static_cast<float>(screenWidth)/static_cast<float>(screenHeight));
	}

	void update(float delta)
	{
		sound_.onUpdate(delta);
		camera_.onUpdate(delta);
		physics_.onUpdate(delta);
		render_->onUpdate(delta);
	}
};