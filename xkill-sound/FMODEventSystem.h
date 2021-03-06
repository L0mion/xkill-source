#pragma once

#include <string>
#include <vector>
#include <fmod_event.hpp>
#include <fmod_errors.h>
#include <xkill-utilities/Math.h>

/** \defgroup xkill-sound XKILL - Sound
 *
 * \brief Sound package of XKILL
 */

/**
 * \class FMODEventSystem
 * \ingroup xkill-sound
 * \brief Wrapper for an FMOD::EventSystem.
 * Stores sound events parsed from an sound event file (.fev).
 * FMOD dll file dependencies:
 * fmodex.dll
 * fmod_event.dll
 * \author (last to touch it) $Author: Henrik Nell $
 */


class FMODEventSystem
{
	friend class FMODEventSystemProgrammerReportParser;

	struct FmodEventStruct
	{
		FmodEventStruct(){};
		FmodEventStruct(int eventIndex, int ownerPlayerEntityId, FMOD::Event* FmodEvent)
		{
			this->eventIndex = eventIndex; 
			this->ownerPlayerEntityId = ownerPlayerEntityId; 
			this->FmodEvent = FmodEvent;
		}

		int eventIndex;
		int ownerPlayerEntityId;
		FMOD::Event* FmodEvent;
	};

public:
	FMODEventSystem(void);
	~FMODEventSystem(void);

	/** \brief Init FMOD event system object, FMOD system object and the sound device.
      * \param mediaPath Paths for: sound event file (.fev), sound bank file (.fsb) and event text file (called "programmer report" in FMOD Designer when building project) (.txt).
	  * \param soundEventFileName Attached to path when loading events from a sound event file (.fev).
	  * \param maxChannels From the FMOD documentation: "The maximum number of channels to be used in FMOD. They are also called 'virtual channels' as you can play as many of these as you want, even if you only have a small number of hardware or software voices. See remarks for more."
      */
	bool Init(std::string mediaPath, std::string soundEventFileName, int maxChannels);

	/** \brief Must be called for the event system to work as intended.*/
	void Update();
	/** \brief Start a sound event at index in the mSoundEvents vector.*/
	void StartSoundEventAt(unsigned int index, int ownerPlayerEntityId, Float3 position, bool use3DAudio);
	/** \brief Stop playing the event at index.*/
	void StopSoundEventAt(unsigned int index, int ownerPlayerEntityId);
	/** \brief Stop all sounds except music.*/
	void StopAllSoundEffects();
	/** \brief Stop all music. */
	void StopAllMusic();
	/** \brief Mutes all sounds.*/
	void SetMuteSounds(bool mute = true);
	/** \brief Set volume.*/
	void SetVolume(float volume);
	/** \brief Set music volume.*/
	void SetMusicVolume(float volume);

	void UpdateNrOfListeners();

	std::vector<std::string> GetFMODEventNames();

private:
	bool FMODErrorCheck(FMOD_RESULT result);
	void update3DListeners();
	FMOD_VECTOR float3ToFModVector(Float3 v);

	FMOD::EventSystem* mEventsystem;
	std::vector<FmodEventStruct> mEvents;
	std::string mSoundEventFileNameWithoutExtension;
	std::string mMediaPath;
	int nrOfEvents_;
	int nrOfListeners_;
};