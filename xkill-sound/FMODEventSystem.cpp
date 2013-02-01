#include "FMODEventSystem.h"
#include <iostream>//print error

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_RELEASE(x) if( x ) { (x)->release(); (x) = NULL; }

FMODEventSystem::FMODEventSystem(void)
{
	mEventsystem = NULL;
	mSoundEventFileNameWithoutExtension = "";
}

FMODEventSystem::~FMODEventSystem(void)
{
	SAFE_RELEASE(mEventsystem);
}

bool FMODEventSystem::FMODErrorCheck(FMOD_RESULT result)
{
	static bool error = false;
	if (!error && result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		error = true;
	}

	return error;
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
}

void FMODEventSystem::Update()
{
	FMODErrorCheck(mEventsystem->update());

	for(unsigned int i = 0; i < mEvents.size(); i++)
	{
		FMOD_EVENT_STATE state;
		if(mEvents[i]->getState(&state) == FMOD_ERR_INVALID_HANDLE)
		{
			FMOD::Event* temp = mEvents[mEvents.size() - 1];
			mEvents[mEvents.size() - 1] = mEvents[i];
			mEvents[i] = temp;

			mEvents.pop_back();
		}
	}
}

void FMODEventSystem::StartSoundEventAt(unsigned int index)
{
	if(index < (unsigned int)nrOfEvents_)
	{
		FMOD::Event* soundEvent;
		FMODErrorCheck(mEventsystem->getEventBySystemID(index, FMOD_EVENT_NONBLOCKING, &soundEvent));
		soundEvent->start();
		mEvents.push_back(soundEvent);
	}
	else
	{
		//std::cout << "error in \"FMODEventSystem.h\" in function \"void FMODEventSystem::StartSoundEventAt(int index)\"" << std::endl;
	}
}

void FMODEventSystem::SetMuteSounds(bool mute)
{
	int nrOfEvents;
	FMOD::Event* soundEvent;
	if(mEventsystem->getNumEvents(&nrOfEvents) == FMOD_OK)
	{
		for(int i = 0; i < nrOfEvents; i++)
		{
			if(mEventsystem->getEventBySystemID(i, FMOD_EVENT_INFOONLY, &soundEvent) == FMOD_OK)
			{
				soundEvent->setMute(mute);
			}		
		}
	}

	for(unsigned int i = 0; i < mEvents.size(); i++)
	{
		mEvents[i]->setMute(mute);
	}
}

void FMODEventSystem::SetVolume(float volume)
{
	int nrOfEvents;
	FMOD::Event* soundEvent;
	if(mEventsystem->getNumEvents(&nrOfEvents) == FMOD_OK)
	{
		for(int i = 0; i < nrOfEvents; i++)
		{
			if(mEventsystem->getEventBySystemID(i, FMOD_EVENT_INFOONLY, &soundEvent) == FMOD_OK)
			{
				soundEvent->setVolume(volume);
			}
		}
	}

	for(unsigned int i = 0; i < mEvents.size(); i++)
	{
		mEvents[i]->setVolume(volume);
	}
}

std::vector<std::string> FMODEventSystem::GetFMODEventNames()
{
	FMOD::Event* e;
	std::vector<std::string> names;

	int nrOfEvents;
	if(!FMODErrorCheck(mEventsystem->getNumEvents(&nrOfEvents)))
	{
		for(int i = 0; i < nrOfEvents; i++)
		{
			FMODErrorCheck(mEventsystem->getEventBySystemID(i, FMOD_EVENT_INFOONLY, &e));

			int nr;
			FMOD_EVENT_INFO info;
			char* name;

			if(!FMODErrorCheck(e->getInfo(&nr, &name, &info)))
			{
				names.push_back(std::string(name));
			}
		}
	}

	return names;
}
