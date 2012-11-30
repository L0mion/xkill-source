#include "SoundComponent.h"

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-sound/FMODEventSystem.h> //check architecture project depending on sound
#include <xkill-sound/FMODEventSystemProgrammerReportParser.h> //check architecture project depending on sound

#include <iostream>

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

SoundComponent::SoundComponent()
{
	mFMODEventSystem = NULL;
}

SoundComponent::~SoundComponent()
{
	SAFE_DELETE(mFMODEventSystem);
}

bool SoundComponent::init()
{
	mFMODEventSystem = new FMODEventSystem();
	mFMODEventSystem->Init("../xkill-dependencies/sound/", "testproject.fev", 64);

	FMODEventSystemProgrammerReportParser fmodEventSystemProgrammerReportParser;
	if(!fmodEventSystemProgrammerReportParser.parseProgrammerReport(mFMODEventSystem))
	{
		std::cout << "parsing of FMOD Designer's programmer's report failed." << std::endl;
		return false;
	}

	return true;
}

void SoundComponent::onEvent(Event* e)
{
	//mFMODEventSystem->StartSoundEventAt(0);
}

void SoundComponent::onUpdate(float delta)
{
	mFMODEventSystem->Update();
}