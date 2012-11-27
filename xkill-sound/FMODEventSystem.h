#pragma once

#include "dllSound.h"

#include <string>
#include <vector>
#include <fmod_event.hpp>
#include <fmod_errors.h>

//#if defined( DEBUG ) || defined( _DEBUG )
//#pragma comment(lib, "../xkill-dependencies/lib/fmodexL_vc.lib")
//#pragma comment(lib, "../xkill-dependencies/lib/fmod_eventL.lib")
//#else
//#pragma comment(lib, "../xkill-dependencies/lib/fmodex_vc.lib")
//#pragma comment(lib, "../xkill-dependencies/lib/fmod_event.lib")
//#endif

//dll files:
//fmodex.dll
//fmod_event.dll

/** \defgroup xkill-sound XKILL - Sound
 *
 * \brief Sound package of XKILL
 */

/**
 * \class FMODEventSystem
 * \ingroup xkill-sound
 * \brief Wrapper for an FMOD::EventSystem.
 * Stores sound event parsed from an sound event file (.fev).
 * \note This is a note.
 * \author (last to touch it) $Author: Henrik Nell $
 */


class DLL_S FMODEventSystem
{
	friend class FMODEventSystemProgrammerReportParser;

public:
	FMODEventSystem(void);
	~FMODEventSystem(void);

	/** \brief Init FMOD event system object, FMOD system object and the sound device.
      * \param mediaPath Paths for: sound event file (.fev), sound bank file (.fsb) and event text file (called "programmer report" in FMOD Designer when building project) (.txt).
	  * \param soundEventFileName Attached to path when loading events from a sound event file (.fev).
	  * \param maxChannels From the FMOD documentation: "The maximum number of channels to be used in FMOD. They are also called 'virtual channels' as you can play as many of these as you want, even if you only have a small number of hardware or software voices. See remarks for more."
      */
	void Init(std::string mediaPath, std::string soundEventFileName, int maxChannels);

	/** \brief Must be called for the event system to work as intended.*/
	void Update();
	void StartSoundEventAt(int index);

private:
	void FMODErrorCheck(FMOD_RESULT result);

	/** \brief Loads a sound event from the sound event file (.fev).*/
	//void loadSoundEvent(std::string pathInsideSoundEventFile);

	FMOD::EventSystem* mEventsystem;
	std::vector<FMOD::Event*>* mSoundEvents;
	std::string mSoundEventFileNameWithoutExtension;
	std::string mMediaPath;
};