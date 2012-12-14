#include "ComponentManager.h"

#include <xkill-io/IOComponent.h>
#include <xkill-renderer/renderingComponent.h>
#include <xkill-physics/BulletPhysicsComponent.h>
#include <xkill-sound/SoundComponent.h>
#include <xkill-input/InputComponent.h>

#include "CameraComponent.h"
#include "GameComponent.h"
#include "ScoreComponent.h"

#include <xkill-utilities/FiniteStateMachine.h>
#include <xkill-utilities/FiniteState.h>
#include "States.h"

#include <iostream>

//#include <windows.h>

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

ComponentManager::ComponentManager()
{
	//SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	//SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);

	render_			= NULL;
	physics_		= NULL;
	sound_			= NULL;
	camera_			= NULL;
	input_			= NULL;
	game_			= NULL;
	score_			= NULL;
	ioComponent_	= NULL;
}
ComponentManager::~ComponentManager()
{
	SAFE_DELETE(render_);
	SAFE_DELETE(physics_);
	SAFE_DELETE(sound_);
	SAFE_DELETE(camera_);
	SAFE_DELETE(input_);
	SAFE_DELETE(game_);
	SAFE_DELETE(score_)
	SAFE_DELETE(ioComponent_);
}

bool ComponentManager::init(HWND windowHandle, HWND parentWindowHandle)
{

//----------ADD STATEMACHINE-----------------------------------------------------------------
	/*
	FiniteState* deathmatchState = new DeathMatchState(STATE_DEATHMATCH);
	FiniteState* mainMenuState = new MainMenuState(STATE_MAINMENU);
	stateMachine_.AddState(deathmatchState);
	stateMachine_.AddState(mainMenuState);
	*/
//------------------------delete "deathmatchState" and "mainMenuState" somewhere //check-----
	
	//Substitute statemchine, enum
	state_TemporaryVariableUsedAsSubstituteForStateMachine = SPECIAL_STATE_NONE;

	render_ = new RenderingComponent(windowHandle);
	physics_ = new BulletPhysicsComponent();
	camera_ = new CameraComponent();
	game_ = new GameComponent();
	sound_ = new SoundComponent();
	input_ = new InputComponent();
	score_ = new ScoreComponent();
	ioComponent_ = new IOComponent();

	if(render_->init() != S_OK)
	{
		std::cout << "RenderingComponent failed to init." << std::endl;
		return false;
	}

	if(!physics_->init())
	{
		std::cout << "BulletPhysicsComponent failed to init." << std::endl;
		return false;
	}

	if(!camera_->init())
	{
		std::cout << "CameraComponent failed to init." << std::endl;
		return false;
	}
	
	if(!game_->init())
	{
		std::cout << "GameComponent failed to init." << std::endl;
		return false;
	}

	std::string configPath = "../../xkill-resources/xkill-configs/";
	if(!sound_->init(configPath))
	{
		std::cout << "SoundComponent failed to init." << std::endl;
		return false;
	}

	if(!input_->init(parentWindowHandle, AttributeManager::getInstance()->inputAttributes_.getAllAttributes(), configPath))
	{
		std::cout << "InputComponent failed to init." << std::endl;
		return false;
	}
		
	if(!score_->init(AttributeManager::getInstance()->playerAttributes_.getAllAttributes()))
	{
		std::cout << "ScoreComponent failed to init." << std::endl;
		return false;
	}

	if(!ioComponent_->init())
	{
		std::cout << "IOComponent failed to init." << std::endl;
		return false;
	}

	// Returns that everything went ok
	return true;
}

void ComponentManager::onEvent(Event* e) 
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_END_DEATHMATCH:
		state_TemporaryVariableUsedAsSubstituteForStateMachine = STATE_MAINMENU;
		break;
	case EVENT_START_DEATHMATCH: //currently sent from GameManager::init()
		state_TemporaryVariableUsedAsSubstituteForStateMachine = STATE_DEATHMATCH;
	default:
		break;
	}
}

void ComponentManager::update(float delta)
{
	if(STATE_DEATHMATCH)
	{
		sound_->onUpdate(delta);
		physics_->onUpdate(delta);
		camera_->onUpdate(delta);
		SEND_EVENT(&Event_DoCulling());
		render_->onUpdate(delta);
		input_->onUpdate(delta);
		game_->onUpdate(delta);
		SEND_EVENT(&Event(EVENT_UPDATE));
	}
	else if(STATE_MAINMENU)
	{
		std::cout << "ComponentManager::update, MAIN MENU" << std::endl;
	}
	else if(SPECIAL_STATE_NONE)
	{
		std::cout << "ComponentManager::update has no state set" << std::endl;
	}
}