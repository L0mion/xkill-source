#include "FMODEventSystemProgrammerReportParser.h"
#include <fstream>

FMODEventSystemProgrammerReportParser::FMODEventSystemProgrammerReportParser(void)
{
}

FMODEventSystemProgrammerReportParser::~FMODEventSystemProgrammerReportParser(void)
{
}

bool FMODEventSystemProgrammerReportParser::parseProgrammerReport(FMODEventSystem* FMODEventSystemWrapperClass)
{
	if(FMODEventSystemWrapperClass==NULL)
	{
		return false;
	}

	FMOD::Event* soundEvent;

	//Path needed by the "programmer's report" parser
	std::string programmersReportPath;
	programmersReportPath += FMODEventSystemWrapperClass->mMediaPath;
	programmersReportPath += FMODEventSystemWrapperClass->mSoundEventFileNameWithoutExtension;
	programmersReportPath += ".txt";

	//Path needed by FMOD::EventSystem::getEvent()
	std::string soundEventPath;
	soundEventPath += FMODEventSystemWrapperClass->mSoundEventFileNameWithoutExtension;

	std::ifstream read(programmersReportPath);
	if(!read.is_open())
		return false;

	std::string pieceOfLine;
	std::string lineFromTextFile;
	while(!read.eof())
	{
		std::getline(read, lineFromTextFile);
		pieceOfLine = lineFromTextFile.substr(0, 4);
		if(pieceOfLine == "Name")
		{
			int indexOfCharacter = lineFromTextFile.find_first_of("/");
			pieceOfLine = lineFromTextFile.substr(indexOfCharacter, std::string::npos);

			FMODEventSystemWrapperClass->FMODErrorCheck(FMODEventSystemWrapperClass->mEventsystem->getEvent( (soundEventPath+pieceOfLine).c_str(), FMOD_EVENT_DEFAULT, &soundEvent));
			//FMODEventSystemWrapperClass->mSoundEvents->push_back(soundEvent);
		}
		else if(lineFromTextFile == "# Groups") //This implementation does not handle event groups
		{
			break;
		}
	}

	read.close();

	return true;
}