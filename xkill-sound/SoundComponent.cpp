#include "SoundComponent.h"

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-sound/FMODEventSystem.h> //check architecture project depending on sound
#include <xkill-sound/FMODEventSystemProgrammerReportParser.h> //check architecture project depending on sound

#include <iostream>

#include "FileParser.h"
#include "EventToFModConverter.h"

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

SoundComponent::SoundComponent()
{
	mFMODEventSystem = NULL;
	converter = NULL;
	timer = 0.0f;

	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYSOUND);
	SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PROJECTILE);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
}

SoundComponent::~SoundComponent()
{
	SAFE_DELETE(mFMODEventSystem);
	SAFE_DELETE(converter);
}

bool SoundComponent::init(std::string configFilePath)
{
	int test = sizeof(*ATTRIBUTE_MANAGER);

	mFMODEventSystem = new FMODEventSystem();
	mFMODEventSystem->Init("../xkill-dependencies/sound/", "Xkill 1.0.fev", 64);

	//FMODEventSystemProgrammerReportParser fmodEventSystemProgrammerReportParser;
	//if(!fmodEventSystemProgrammerReportParser.parseProgrammerReport(mFMODEventSystem))
	//{
	//	DEBUGPRINT("parsing of FMOD Designer's programmer's report failed.");
	//	return false;
	//}

	converter = new EventToFModConverter();
	converter->init(configFilePath);

	fillEventsToFModVector(configFilePath);

	int fmodEventIndex = converter->getFModIndex(4);
	if(fmodEventIndex >= 0)
		mFMODEventSystem->StartSoundEventAt(fmodEventIndex);

	return true;
}

void SoundComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	int eventIndex = -1;

	if(type == EventType::EVENT_PLAYSOUND)
	{
		Event_PlaySound* eps = static_cast<Event_PlaySound*>(e);
		
		if(eps->muteSound)
			mFMODEventSystem->SetMuteSounds();
		else
			eventIndex = eps->soundId;
	}
	else
	{
		eventIndex = (int)type;
	}

	int fmodEventIndex = converter->getFModIndex(eventIndex);
	if(fmodEventIndex >= 0)
		mFMODEventSystem->StartSoundEventAt(fmodEventIndex);
}

void SoundComponent::onUpdate(float delta)
{
	//timer += delta;
	//if(timer >= 0.5f)
	//{
	//	timer = 0.0f;
	//	int fmodEventIndex = converter->getFModIndex(3);
	//	if(fmodEventIndex >= 0)
	//		mFMODEventSystem->StartSoundEventAt(fmodEventIndex);
	//}

	mFMODEventSystem->Update();
}

void SoundComponent::fillEventsToFModVector(std::string configFilePath)
{
	FileParser fp(configMessage());
	fp.setFileName("sound.cfg");
	fp.setFilePath(configFilePath);
	if(fp.startReading())
	{
		while(!fp.isEmpty())
		{
			std::string tmp = fp.getNextRow();
			converter->addConversion(tmp);
		}
	}
}

std::string SoundComponent::configMessage()
{
	std::string message = "";

	message += "// Define a binding between events by using this format\n";
	message += "// <Fmod event number> = <game event number>\n";
	message += "// <Fmod event number> = <game event name>\n";
	message += "// Example:\n";
	message += "// 0 = 4";
	message += "// This will bind fmod event '0' to game event '4'\n";
	message += "// \n";
	message += "// 1 = CreateProjectile\n";
	message += "// This will bind fmod event '1' to the 'CreateProjectile' game event\n";
	message += "// Event names can be found in 'events.cfg'\n";

	return message;
}