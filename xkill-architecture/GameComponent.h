#pragma once
#include <xkill-utilities/IObserver.h>

class GameComponent :
	public IObserver
{
public:
	GameComponent(void);
	~GameComponent(void);
	bool init();

	void onEvent(Event* e);
	void onUpdate(float delta);
};