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

	SUBSCRIBE_TO_EVENT(this, EVENT_CREATEPROJECTILE);
}

SoundComponent::~SoundComponent()
{
	SAFE_DELETE(mFMODEventSystem);
	SAFE_DELETE(converter);
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

	converter = new EventToFModConverter();

	fillEventsToFModVector();

	return true;
}

void SoundComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	mFMODEventSystem->StartSoundEventAt(converter->getFModIndex((int)type));
}

void SoundComponent::onUpdate(float delta)
{
	mFMODEventSystem->Update();
}

void SoundComponent::fillEventsToFModVector()
{
	FileParser fp(configMessage());
	fp.setFileName("sound.cfg");
	fp.setFilePath("../../xkill-resources/xkill-configs/");
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
	message += "// Example:\n";
	message += "// 0 = 4";
	message += "// This will bind fmod event '0' to game event '4'";

	return message;
}