#include "SoundComponent.h"

#include <xkill-utilities/XKILL_Enums.h>
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/Util.h>
#include "FMODEventSystem.h"

#include "FileParser.h"
#include "EventToFModConverter.h"

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

ATTRIBUTES_DECLARE_ALL

SoundComponent::SoundComponent()
{
	ATTRIBUTES_INIT_ALL

	useSound = false;
	mFMODEventSystem = NULL;
	converter = NULL;
	timer = 0.0f;

	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYSOUND);
	SUBSCRIBE_TO_EVENT(this, EVENT_STOPSOUND);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATESOUNDSETTINGS);
}

SoundComponent::~SoundComponent()
{
	SAFE_DELETE(mFMODEventSystem);
	SAFE_DELETE(converter);
}

bool SoundComponent::init(std::string configFilePath)
{
	mFMODEventSystem = new FMODEventSystem();
	useSound = mFMODEventSystem->Init("../../xkill-resources/xkill-sounds/", "Xkill_Sound.fev", 64);

	//FMODEventSystemProgrammerReportParser fmodEventSystemProgrammerReportParser;
	//if(!fmodEventSystemProgrammerReportParser.parseProgrammerReport(mFMODEventSystem))
	//{
	//	DEBUGPRINT("parsing of FMOD Designer's programmer's report failed.");
	//	return false;
	//}

	if(useSound)
	{
		converter = new EventToFModConverter(mFMODEventSystem->GetFMODEventNames());
		converter->init(configFilePath);

		fillEventsToFModVector(configFilePath);
	}

	return true;
}

void SoundComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	int eventIndex = -1;

	if(!useSound)
		return;

	if(type == EventType::EVENT_PLAYSOUND)
	{
		Event_PlaySound* eps = static_cast<Event_PlaySound*>(e);
		
		eventIndex = eps->soundId;

		int fmodEventIndex = converter->getFModIndex(eventIndex);
		if(fmodEventIndex >= 0)
			mFMODEventSystem->StartSoundEventAt(fmodEventIndex, eps->position, eps->use3DAudio);
	}
	else if(type == EventType::EVENT_UPDATESOUNDSETTINGS)
	{
		//Attribute_SoundSettings* soundSettings;
		//while(itrSoundSettings.hasNext())
		//{
		//	soundSettings = itrSoundSettings.getNext();
		//
		//	mFMODEventSystem->SetMuteSounds(soundSettings->soundMuted);
		//	mFMODEventSystem->SetVolume(soundSettings->soundVolume);
		//}
		mFMODEventSystem->SetMuteSounds(SETTINGS->soundMuted);
		mFMODEventSystem->SetVolume(SETTINGS->soundVolume);
	}
	else if(type == EventType::EVENT_START_DEATHMATCH)
	{
		mFMODEventSystem->UpdateNrOfListeners();
	}
	else if(type == EventType::EVENT_STOPSOUND)
	{
		Event_PlaySound* eps = static_cast<Event_PlaySound*>(e);
		
		eventIndex = eps->soundId;

		int fmodEventIndex = converter->getFModIndex(eventIndex);
		if(fmodEventIndex >= 0)
			mFMODEventSystem->StopSoundEventAt(fmodEventIndex);
	}
}

void SoundComponent::onUpdate(float delta)
{
	if(!useSound)
		return;

	//timer += delta;
	//if(timer >= 0.5f)
	//{
	//	timer = 0.0f;
	//	int fmodEventIndex = converter->getFModIndex(XKILL_Enums::Sound::SOUND_WALK);
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
	message += "// <Fmod event name/number> = <game event name/number>\n";
	message += "// Example:\n";
	message += "// 0 = 4";
	message += "// This will bind fmod event '0' to game event '4'\n";
	message += "// \n";
	message += "// 1 = CreateProjectile\n";
	message += "// This will bind fmod event '1' to the 'CreateProjectile' game event\n";
	message += "// Event names can be found in 'events.cfg'\n";

	return message;
}