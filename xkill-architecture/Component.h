#ifndef COMPONENT_H
#define COMPONENT_H

#include <Windows.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#define FUNCDEF( out , name , in) typedef out (*name##Func)(in);			\
								  name##Func _##name##Func
#define QUOTE "\""
#define FUNCLOAD( name, name2 )  _##name##Func = (name##Func)GetProcAddress(lib, name2)
#define SAFECALL( cmd ) load(); if(lib){ cmd }

class Component
{
	HINSTANCE lib;
	std::string filename;
	__time64_t lastchange;
	FUNCDEF( bool, init, void);
	FUNCDEF( void, onUpdate, float);
	FUNCDEF( void, onEvent, void);

	void unload();
public:
	Component(std::string filename);
	~Component();
	void load();
	bool init();
	void onUpdate(float delta);
	void onEvent();
};
#endif