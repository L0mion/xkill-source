#pragma once
#include <xkill-utilities/IObserver.h>

class Event_ProjectileCollidingWithPlayer;
class GameComponent :
	public IObserver
{
private:

public:
	GameComponent(void);
	~GameComponent(void);
	bool init();

	void onEvent(Event* e);
	void onUpdate(float delta);

	
	void event_EntitiesColliding(Event_ProjectileCollidingWithPlayer* e);
};