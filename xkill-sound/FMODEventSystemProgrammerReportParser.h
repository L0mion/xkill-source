#pragma once
#include "FMODEventSystem.h"

/**
 * \class FMODEventSystemProgrammerReportParser
 * \ingroup xkill-sound
 * \brief Parses a "Programmer's report" (.txt) (optional FMOD Designer output)
 * \note friend class of "FMODEventSystem".
 * \author (last to touch it) $Author: Henrik Nell $
 */
class DLL FMODEventSystemProgrammerReportParser
{
public:
	FMODEventSystemProgrammerReportParser(void);
	~FMODEventSystemProgrammerReportParser(void);

	/** \brief Load sound events from sound event file (.fev). The paths inside the sound event file is found by parsing the "programmer's report" (.txt) generated when building a FMOD Designer project.
      * \param eventSystem Loads sound events into this.
      */
	bool parseProgrammerReport(FMODEventSystem* eventSystem);
};