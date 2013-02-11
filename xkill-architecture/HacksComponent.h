#pragma once

#include <xkill-utilities/IObserver.h>
#include <vector>
#include <xkill-utilities/XKILL_Enums.h>

struct Attribute_Player;
template <class T> class AttributePtr;
class Timer;

class Event_HackActivated;

class HacksComponent : public IObserver
{
public:
	HacksComponent();
	~HacksComponent();

	virtual bool init();

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	std::vector<std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>> activeHacks_;

	void handleHack(AttributePtr<Attribute_Player>& player, XKILL_Enums::HackType hackType);

	void handleHackActivatedEvent(Event_HackActivated* e);

	void removeIndexFromVector(std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>& vector, unsigned int index);
};