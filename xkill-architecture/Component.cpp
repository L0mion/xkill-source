#include "Component.h"

Component::Component(std::string filename)
{
	this->lib = 0;
	this->filename = filename;
}
Component::~Component()
{
	if(lib)
		unload();
}
void Component::load()
{
	struct __stat64 filestat;
	int result;
	if(lib)
	{
		std::string cmd = "dlls\\";
		cmd += filename;
		result = _stat64(cmd.c_str(),&filestat);
		if(result != 0)
		{
			return;
		}
		else if(filestat.st_mtime == lastchange)
		{
			return;
		}
		unload();
	}
	

	std::string tmp = "tmp";
	tmp += filename;

	std::string cmd = "copy /Y \"dlls\\";
	cmd += filename;
	cmd += "\" \"dlls\\";
	cmd += tmp;
	cmd += "\"";
	//system("dir");
	system(cmd.c_str());
	
	cmd = "dlls\\";
	cmd += tmp;
	lib = LoadLibraryA(cmd.c_str());//tmp.c_str()

	FUNCLOAD(init,"iinit");
	FUNCLOAD(onUpdate, "ionUpdate");
	FUNCLOAD(onEvent, "ionEvent");
	cmd = "dlls\\";
	cmd += filename;
	_stat64(cmd.c_str(),&filestat);
	lastchange = filestat.st_mtime;
}
void Component::unload()
{
	FreeLibrary(lib);
	lib = 0;
	std::string cmd = "del \"dlls\\tmp";
	cmd.append(filename);
	cmd.append("\"");
	system(cmd.c_str());
}
bool Component::init()
{
	SAFECALL( return _initFunc(); )
}
void Component::onUpdate(float delta)
{
	SAFECALL( _onUpdateFunc(delta); )
}
void Component::onEvent()
{
	SAFECALL( _onEventFunc(); )
}