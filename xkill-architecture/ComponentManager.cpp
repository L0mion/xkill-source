#include "ComponentManager.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-io/IOComponent.h>
#include <xkill-renderer/renderingComponent.h>
#include <xkill-physics/PhysicsComponent.h>
#include <xkill-sound/SoundComponent.h>
#include <xkill-input/InputComponent.h>

#include "CameraComponent.h"
#include "GameComponent.h"
#include "ScoreComponent.h"
#include "HacksComponent.h"
#include "CullingComponent.h"

//#include <xkill-utilities/FiniteStateMachine.h>
//#include <xkill-utilities/FiniteState.h>
//#include "States.h"

//// timer defines

#define XKILLPROFILING // commment away to skip profiling
#ifdef XKILLPROFILING
#include <xkill-utilities\Converter.h>
#include <time.h>
#include <iostream>
#include <Windows.h>
static float outside;
static std::vector<float> inputtimer;
static std::vector<float> physicstimer;
static std::vector<float> cameratimer;
static std::vector<float> gametimer;
static std::vector<float> soundtimer;
static std::vector<float> hackstimer;
static std::vector<float> scoretimer;
static std::vector<float> rendertimer;
static std::vector<float> cullingtimer;
static std::vector<float> totaltimer;
#define calctime(vectorname, call ) {  clock_t deltatimevar = clock();	\
							call \
							vectorname.push_back(((float)(clock()-deltatimevar))/((float)CLOCKS_PER_SEC));} 
#define outputaverage(outname, vectorname) {float sum=0; for(unsigned int i=0;i<vectorname.size();i++) { sum += vectorname.at(i);} std::string out = outname;  out +=" "; sum = sum/(float)vectorname.size(); out += Converter::FloatToStr(sum); out +=" "; out += Converter::IntToStr((int)(100.0f*sum/totalAverage));  out +="\n"; OutputDebugStringA(out.c_str()); }
#else
#define calctime(vectorname, call ) call
#define outputaverage(outname, vectorname)
#endif

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

ComponentManager::ComponentManager()
{
	SUBSCRIBE_TO_EVENT(this, EVENT_GAMEOVER);
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
#ifdef XKILLPROFILING
	float totalAverage = 0; for(unsigned int i=0;i<totaltimer.size();i++) { totalAverage += totaltimer.at(i);} totalAverage = totalAverage/(float)totaltimer.size();
	outputaverage("input",	inputtimer)
	outputaverage("camera",	cameratimer)
	outputaverage("game",	gametimer)
	outputaverage("sound",	soundtimer)
	outputaverage("hacks",	hackstimer)
	outputaverage("score",	scoretimer)
	outputaverage("physics",physicstimer)
	outputaverage("render",	rendertimer)
	outputaverage("culling",cullingtimer)
	outputaverage("total",	totaltimer)
	std::string out = Converter::FloatToStr(totaltimer.size()*totalAverage);
	out += "\n";
	out += Converter::FloatToStr(outside);
	OutputDebugStringA(out.c_str());
#endif

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
	cullingComponent_ = new CullingComponent();

	if(!(render_->init()))
	{
		ERROR_MESSAGEBOX("RenderingComponent failed to init.");
		return false;
	}

	if(!camera_->init())
	{
		ERROR_MESSAGEBOX("CameraComponent failed to init.");
		return false;
	}
	
	if(!game_->init())
	{
		ERROR_MESSAGEBOX("GameComponent failed to init.");
		return false;
	}

	std::string configPath = "../../xkill-resources/xkill-configs/";
	if(!sound_->init(configPath))
	{
		ERROR_MESSAGEBOX("SoundComponent failed to init.");
		return false;
	}

	if(!input_->init(parentWindowHandle, configPath))
	{
		ERROR_MESSAGEBOX("InputComponent failed to init.");
		return false;
	}
		
	if(!score_->init())
	{
		ERROR_MESSAGEBOX("ScoreComponent failed to init.");
		return false;
	}

	if(!hacks_->init())
	{
		ERROR_MESSAGEBOX("HacksComponent failed to init.");
		return false;
	}

	if(!ioComponent_->init())
	{
		ERROR_MESSAGEBOX("IOComponent failed to init.");
		return false;
	}

	if(!physics_->init())
	{
		ERROR_MESSAGEBOX("BulletPhysicsComponent failed to init.");
		return false;
	}

	if(!cullingComponent_->init())
	{
		ERROR_MESSAGEBOX("CullingComponent failed to init.");
		return false;
	}

	SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_MENU_MUSIC, Float3(), false));
	inputDeviceSearchTime_ = 0;
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
	case EVENT_GAMEOVER:
		gameOverDelay_ = 10.0f;
		GET_STATE() = STATE_GAMEOVER;
		break;
	default:
		break;
	}
}

void updateCamera()
{
	AttributeIterator<Attribute_Camera> itrCamera = ATTRIBUTE_MANAGER->camera.getIterator();
	while(itrCamera.hasNext())
	{
		itrCamera.getNext()->syncSpatialWithAim();
	}
}

void updateOffset()
{
	AttributeIterator<Behavior_Offset> itrOffset = ATTRIBUTE_MANAGER->offset.getIterator();
	while(itrOffset.hasNext())
	{
		AttributePtr<Behavior_Offset> ptr_offset = itrOffset.getNext();
		ptr_offset->updateOffset();
	}
}





void ComponentManager::update(float delta)
{
	// Performs necessary per-frame updating of some sub-parts of EventManager.
	EventManager::getInstance()->update(delta);


	//// PUT SOMETHING 
	/// DONT SPAWN PLAYERS FIRST FRAMES
	/// PUT SOMETHING

	if(GET_STATE() == STATE_DEATHMATCH)
	{
#ifdef XKILLPROFILING
		static clock_t deltatimevartotal2;
		clock_t deltatimevartotal = clock();
		outside += ((float)(deltatimevartotal-deltatimevartotal2))/((float)CLOCKS_PER_SEC);
#endif
		calctime(inputtimer,input_->onUpdate(delta);)
		calctime(physicstimer,physics_->onUpdate(delta);)
		updateOffset();
		calctime(cameratimer,camera_->onUpdate(delta);)
		updateCamera();
		updateOffset();
		
		calctime(gametimer,game_->onUpdate(delta);)

		calctime(soundtimer,sound_->onUpdate(delta);)
		calctime(hackstimer,hacks_->onUpdate(delta);)

		calctime(scoretimer,score_->onUpdate(delta);)
		calctime(cullingtimer, cullingComponent_->onUpdate(delta);)
		calctime(rendertimer,render_->onUpdate(delta);)
	
#ifdef XKILLPROFILING
		deltatimevartotal2 = clock();
		totaltimer.push_back(((float)(deltatimevartotal2-deltatimevartotal))/((float)CLOCKS_PER_SEC));
#endif

		SEND_EVENT(&Event(EVENT_UPDATE));
	}
	else if(GET_STATE() == STATE_GAMEOVER)
	{
		sound_->onUpdate(delta);
		input_->onUpdate(delta);
		render_->onUpdate(delta);

		if(gameOverDelay_ > 0.0f)
		{
			gameOverDelay_ -= delta;
		}
		else
		{
			SEND_EVENT(&Event_EndDeathmatch());
		}
	}
	else if(GET_STATE() == STATE_MAINMENU)
	{
		if(inputDeviceSearchTime_ <= 0)
		{
			SEND_EVENT(&Event(EVENT_INPUT_DEVICE_SEARCH));
			inputDeviceSearchTime_ = 1.0f;	//Do a search every second
		}

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
