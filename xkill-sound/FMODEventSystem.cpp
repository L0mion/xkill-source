#include "FMODEventSystem.h"
#include <iostream>//print error

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_RELEASE(x) if( x ) { (x)->release(); (x) = NULL; }

FMODEventSystem::FMODEventSystem(void)
{
	mEventsystem = NULL;
	mSoundEvents = NULL;
	mSoundEventFileNameWithoutExtension = "";
}

FMODEventSystem::~FMODEventSystem(void)
{
	SAFE_RELEASE(mEventsystem);
	SAFE_DELETE(mSoundEvents); //All FMOD::Event* are deallocated by FMOD through FMOD::EventSystem
}

void FMODEventSystem::FMODErrorCheck(FMOD_RESULT result)
{
	static bool error = false;
	if (!error && result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		error = true;

	}
}

void FMODEventSystem::Init(std::string mediaPath, std::string soundEventFileName, int maxChannels)
{
	FMODErrorCheck(FMOD::EventSystem_Create(&mEventsystem));
	FMODErrorCheck(mEventsystem->init(maxChannels, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
	
	FMODErrorCheck(mEventsystem->setMediaPath(mediaPath.c_str()));//check
	FMODErrorCheck(mEventsystem->load(soundEventFileName.c_str(), 0, 0));

	mSoundEventFileNameWithoutExtension = soundEventFileName.erase(soundEventFileName.size()-4, soundEventFileName.size());
	mMediaPath = mediaPath;

	FMODErrorCheck(mEventsystem->getNumEvents(&nrOfEvents_));

	mSoundEvents = new std::vector<FMOD::Event*>();
}

void FMODEventSystem::Update()
{
	FMODErrorCheck(mEventsystem->update());
}

void FMODEventSystem::StartSoundEventAt(unsigned int index)
{
	if(index < nrOfEvents_)
	{
		FMOD::Event* soundEvent;
		FMODErrorCheck(mEventsystem->getEventBySystemID(index, FMOD_EVENT_NONBLOCKING, &soundEvent));
		mSoundEvents->push_back(soundEvent);
		soundEvent->start();
	}
	else
	{
#ifdef XKILL_DEBUG
		std::cout << "No sound event with index: " << index << " In class: FMODEventSystem " << std::endl;
#endif
	}
}