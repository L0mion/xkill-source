#include "ComponentManager.h"

#include <xkill-io/IOComponent.h>
#include <xkill-renderer/renderingComponent.h>
#include <xkill-physics/PhysicsComponent.h>
#include <xkill-sound/SoundComponent.h>
#include <xkill-input/InputComponent.h>

#include "CameraComponent.h"
#include "GameComponent.h"
#include "ScoreComponent.h"
#include "HacksComponent.h"

#include <xkill-utilities/FiniteStateMachine.h>
#include <xkill-utilities/FiniteState.h>
#include "States.h"

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

ComponentManager::ComponentManager()
{
	SUBSCRIBE_TO_EVENT(this, EVENT_GAME_OVER);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);

	render_			= NULL;
	physics_		= NULL;
	sound_			= NULL;
	camera_			= NULL;
	input_			= NULL;
	game_			= NULL;
	score_			= NULL;
	hacks_			= NULL;
	ioComponent_	= NULL;
}
ComponentManager::~ComponentManager()
{
	//
	// Clean up behind ourselves like good little programmers
	//

	SAFE_DELETE(render_);
	SAFE_DELETE(physics_);
	SAFE_DELETE(sound_);
	SAFE_DELETE(camera_);
	SAFE_DELETE(input_);
	SAFE_DELETE(game_);
	SAFE_DELETE(score_);
	SAFE_DELETE(hacks_);
	SAFE_DELETE(ioComponent_);
}

bool ComponentManager::init(HWND windowHandle, HWND parentWindowHandle)
{

//----------ADD STATEMACHINE-----------------------------------------------------------------
	/*
	FiniteState* deathmatchState = new DeathMatchState(STATE_DEATHMATCH);
	FiniteState* mainMenuState = new MainMenuState(STATE_MAINMENU);
	stateMachine_.AddState(deathmatchState);_
	stateMachine_.AddState(mainMenuState);
	*/
//------------------------delete "deathmatchState" and "mainMenuState" somewhere //check-----
	
	//Substitute statemachine, enum
	//state_TemporaryVariableUsedAsSubstituteForStateMachine = SPECIAL_STATE_NONE;

	render_ = new RenderingComponent(windowHandle);
	physics_ = new PhysicsComponent();
	camera_ = new CameraComponent();
	game_ = new GameComponent();
	sound_ = new SoundComponent();
	input_ = new InputComponent();
	score_ = new ScoreComponent();
	hacks_ = new HacksComponent();
	ioComponent_ = new IOComponent();

	if(FAILED((render_->init())))
	{
		SHOW_MESSAGEBOX("RenderingComponent failed to init.");
		return false;
	}

	if(!camera_->init())
	{
		SHOW_MESSAGEBOX("CameraComponent failed to init.");
		return false;
	}
	
	if(!game_->init())
	{
		SHOW_MESSAGEBOX("GameComponent failed to init.");
		return false;
	}

	std::string configPath = "../../xkill-resources/xkill-configs/";
	if(!sound_->init(configPath))
	{
		SHOW_MESSAGEBOX("SoundComponent failed to init.");
		return false;
	}

	if(!input_->init(parentWindowHandle, configPath))
	{
		SHOW_MESSAGEBOX("InputComponent failed to init.");
		return false;
	}
		
	if(!score_->init())
	{
		SHOW_MESSAGEBOX("ScoreComponent failed to init.");
		return false;
	}

	if(!hacks_->init())
	{
		SHOW_MESSAGEBOX("HacksComponent failed to init.");
		return false;
	}

	if(!ioComponent_->init())
	{
		SHOW_MESSAGEBOX("IOComponent failed to init.");
		return false;
	}

	if(!physics_->init())
	{
		SHOW_MESSAGEBOX("BulletPhysicsComponent failed to init.");
		return false;
	}	

	//SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_MUSIC, Float3(), false));

	// Returns that everything went ok
	return true;
}

void ComponentManager::onEvent(Event* e) 
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_END_DEATHMATCH:
		GET_STATE() = STATE_MAINMENU;
		break;
	case EVENT_GAME_OVER:
		gameOverDelay = 10.0f;
		GET_STATE() = STATE_GAMEOVER;
		break;
	default:
		break;
	}
}

void ComponentManager::update(float delta)
{
	//// PUT SOMETHING 
	/// DONT SPAWN PLAYERS FIRST FRAMES
	/// PUT SOMETHING
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		input_->onUpdate(delta);
		physics_->onUpdate(delta);
		game_->onUpdate(delta);	
		camera_->onUpdate(delta);
		sound_->onUpdate(delta);
		render_->onUpdate(delta);
		score_->onUpdate(delta);
		hacks_->onUpdate(delta);
	
		SEND_EVENT(&Event(EVENT_UPDATE));
	}
	else if(GET_STATE() == STATE_GAMEOVER)
	{
		sound_->onUpdate(delta);
		input_->onUpdate(delta);
		render_->onUpdate(delta);

		if(gameOverDelay > 0.0f)
		{
			gameOverDelay -= delta;
		}
		else
		{
			SEND_EVENT(&Event_EndDeathmatch());
		}
	}
	else if(GET_STATE() == STATE_MAINMENU)
	{
		SEND_EVENT(&Event(EVENT_INPUT_DEVICE_SEARCH));

		sound_->onUpdate(delta);
		input_->onUpdate(delta);
		render_->onUpdate(delta);
		SEND_EVENT(&Event(EVENT_UPDATE));
	}
	else if(GET_STATE() == SPECIAL_STATE_NONE)
	{
		DEBUGPRINT("ComponentManager::update has no state set");
	}
}
