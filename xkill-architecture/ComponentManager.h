#pragma once

#include <xkill-physics/BulletPhysicsComponent.h>
#include <xkill-sound/SoundComponent.h>
#include <xkill-renderer/renderingComponent.h>
#include <xkill-input/InputComponent.h>

#include "CameraComponent.h"
#include "GameComponent.h"
#include "ScoreComponent.h"
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
	BulletPhysicsComponent physics_;
	SoundComponent		sound_;
	CameraComponent		camera_;
	InputComponent		input_;
	GameComponent		game_;
	ScoreComponent		score_;

public:
	ComponentManager()
	{
		render_ = NULL;
	}
	~ComponentManager()
	{
		SAFE_DELETE(render_);
		//SAFE_DELETE(physicsComponent);
		//SAFE_DELETE(soundComponent);
		//SAFE_DELETE(gameComponent);
	}

	bool init(HWND windowHandle, HWND parentWindowHandle)
	{
		render_ = new RenderingComponent(windowHandle);
		render_->init();
		physics_.init();
		camera_.init();

		if(!game_.init())
			return false;
		if(!sound_.init())
			return false;
		if(!input_.init(parentWindowHandle, AttributeManager::getInstance()->inputAttributes_.getAllAttributes()))
			return false;

		if(!score_.init(AttributeManager::getInstance()->playerAttributes_.getAllAttributes()))
			return false;

		// Returns that everything went ok
		return true;
	}
	void update(float delta)
	{
		//sound_.onUpdate(delta);
		physics_.onUpdate(delta);
		camera_.onUpdate(delta);
		render_->onUpdate(delta);
		input_.onUpdate(delta);
		game_.onUpdate(delta);
	}
};