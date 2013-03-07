#include "FMODEventSystem.h"

#include <xkill-utilities/AttributeManager.h>

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_RELEASE(x) if( x ) { (x)->release(); (x) = NULL; }

ATTRIBUTES_DECLARE_ALL

FMODEventSystem::FMODEventSystem(void)
{
	ATTRIBUTES_INIT_ALL

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

bool FMODEventSystem::Init(std::string mediaPath, std::string soundEventFileName, int maxChannels)
{
	if(FMODErrorCheck(FMOD::EventSystem_Create(&mEventsystem)))
		return false;

	if(FMODErrorCheck(mEventsystem->init(maxChannels, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL)))
		return false;
	
	if(FMODErrorCheck(mEventsystem->setMediaPath(mediaPath.c_str())))
		return false;

	if(FMODErrorCheck(mEventsystem->load(soundEventFileName.c_str(), 0, 0)))
		return false;

	mSoundEventFileNameWithoutExtension = soundEventFileName.erase(soundEventFileName.size()-4, soundEventFileName.size());
	mMediaPath = mediaPath;

	if(FMODErrorCheck(mEventsystem->getNumEvents(&nrOfEvents_)))
		return false;

	UpdateNrOfListeners();

	return true;
}

void FMODEventSystem::Update()
{
	if(nrOfListeners_ > 0 || nrOfListeners_ <= 4)
	{
		update3DListeners();
	}

	FMODErrorCheck(mEventsystem->update());

	for(unsigned int i = 0; i < mEvents.size(); i++)
	{
		FMOD_EVENT_STATE state;

		if(mEvents[i].first->getState(&state) == FMOD_ERR_INVALID_HANDLE)
		{

			FMOD::Event* temp = mEvents[mEvents.size() - 1].first;
			mEvents[mEvents.size() - 1].first = mEvents[i].first;
			mEvents[i].first = temp;

			mEvents.pop_back();
		}
	}
}

void FMODEventSystem::StartSoundEventAt(unsigned int index, Float3 position, bool use3DAudio)
{
	if(index < (unsigned int)nrOfEvents_)
	{
		FMOD::Event* soundEvent;
		FMODErrorCheck(mEventsystem->getEventBySystemID(index, FMOD_EVENT_NONBLOCKING, &soundEvent));
		if(use3DAudio)
		{
			if(nrOfListeners_ > 0 && nrOfListeners_ <= 4)
			{
				soundEvent->set3DAttributes(&float3ToFModVector(position), NULL);
			}
			else
			{
				int fmodProperty = FMOD_2D;
				FMODErrorCheck(soundEvent->setPropertyByIndex(FMOD_EVENTPROPERTY_MODE, &fmodProperty, true));
			}
		}
		soundEvent->start();
		mEvents.push_back(std::pair<FMOD::Event*, int>(soundEvent, index));
	}
	else
	{
		//std::cout << "error in \"FMODEventSystem.h\" in function \"void FMODEventSystem::StartSoundEventAt(int index)\"" << std::endl;
	}
}

void FMODEventSystem::StopSoundEventAt(unsigned int index)
{
	for(unsigned int i = 0; i < mEvents.size(); i++)
	{
		if(mEvents[i].second == index)
		{
			mEvents[i].first->stop();

			FMOD::Event* temp = mEvents[mEvents.size() - 1].first;
			mEvents[mEvents.size() - 1].first = mEvents[i].first;
			mEvents[i].first = temp;

			mEvents.pop_back();
			i--;
		}
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
		mEvents[i].first->setMute(mute);
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
		mEvents[i].first->setVolume(volume);
	}
}

void FMODEventSystem::UpdateNrOfListeners()
{
	nrOfListeners_ = itrPlayer.count();

	if(nrOfListeners_ <= 0)
	{
		return;
	}
	else if(nrOfListeners_ > 4)
	{
		if(FMODErrorCheck(mEventsystem->set3DNumListeners(1)))
			return;
	}
	else
	{
		if(FMODErrorCheck(mEventsystem->set3DNumListeners(nrOfListeners_)))
			return;
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

void FMODEventSystem::update3DListeners()
{
	if(nrOfListeners_ > 4)
		return;

	int i = 0;

	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();
		FMOD_VECTOR pos, look, up;
		Float3 float3_pos = ptr_player->ptr_render->ptr_spatial->ptr_position->position;
		Float3 float3_look = ptr_player->ptr_camera->look;
		Float3 float3_up = ptr_player->ptr_camera->up;

		pos = float3ToFModVector(float3_pos);
		look = float3ToFModVector(float3_look);
		up = float3ToFModVector(float3_up);

		FMODErrorCheck(mEventsystem->set3DListenerAttributes(i++, &pos, NULL, &look, &up));
	}

	itrPlayer.resetIndex();
}

FMOD_VECTOR FMODEventSystem::float3ToFModVector(Float3 v)
{
	FMOD_VECTOR fmodVector;

	fmodVector.x = v.x;
	fmodVector.y = v.y;
	fmodVector.z = v.z;

	return fmodVector;
}