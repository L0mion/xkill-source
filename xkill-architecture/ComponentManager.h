#pragma once

#include <xkill-physics/BulletPhysicsComponent.h>
#include <xkill-sound/SoundComponent.h>
#include <xkill-renderer/renderingComponent.h>
#include <xkill-input/InputComponent.h>

#include "CameraComponent.h"
#include "GameComponent.h"
#include "AttributeManager.h"


/// Is responsible for updating Components in a certain order
/** 
Some \ref COMPONENTS such as RenderComponent and PhysicsComponent need to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order. The InputComponent 
should run before the RenderComponent to hide latency. This should 
be the responsibility of the ComponentManager.

\ingroup ARCHITECTURE
*/

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

class ComponentManager
{
private:
	RenderingComponent* render_;
	BulletPhysicsComponent* physics_;
	//SoundComponent		sound_;
	CameraComponent		camera_;
	InputComponent		input_;
	GameComponent		game_;

public:
	ComponentManager()
	{
		render_ = NULL;
	}
	~ComponentManager()
	{
		SAFE_DELETE(render_);
		SAFE_DELETE(physics_);
		//SAFE_DELETE(physicsComponent);
		//SAFE_DELETE(soundComponent);
		//SAFE_DELETE(gameComponent);
	}

	bool init(HWND windowHandle, unsigned int screenWidth, unsigned int screenHeight)
	{
		render_ = new RenderingComponent(windowHandle,screenWidth,screenHeight,
										AttributeManager::getInstance()->cameraAttributes_.getAllAttributes()->size(),
										AttributeManager::getInstance()->renderAttributes_.getAllAttributes(),
										AttributeManager::getInstance()->cameraAttributes_.getAllAttributes());
		render_->init();
		physics_ = new BulletPhysicsComponent(AttributeManager::getInstance()->physicsAttributes_.getAllAttributes(),
											  AttributeManager::getInstance()->boundingAttributes_.getAllAttributes(),
											  AttributeManager::getInstance()->inputAttributes_.getAllAttributes());
		physics_->init();
		camera_.init(AttributeManager::getInstance()->cameraAttributes_.getAllAttributes(),
					AttributeManager::getInstance()->cameraMovementAttribute_.getAllAttributes(),
					static_cast<float>(screenWidth)/static_cast<float>(screenHeight));

		//gameComponent = new GameComponent();
		if(!game_.init())
			return false;

		//soundComponent = new SoundComponent();
		//if(!sound_.init())
			//return false;

		if(!input_.init(windowHandle, AttributeManager::getInstance()->inputAttributes_.getAllAttributes()))
			return false;

		//physicsComponent = new PhysicsComponent();
		//if(!physics.init(attributes))
		//	return false;
		//std::vector<PhysicsAttribute>* attributes = AttributeManager::getInstance()->physicsAttributes.getAllAttributes();

		return true;
	}
	void update(float delta)
	{
		//sound_.onUpdate(delta);
		camera_.onUpdate(delta);
		physics_->onUpdate(delta);
		render_->onUpdate(delta);
		input_.onUpdate(delta);
		game_.onUpdate(delta);
	}
};