#pragma once

#include <xkill-utilities/IObserver.h>
#include <vector>
#include <xkill-utilities/XKILL_Enums.h>

struct Attribute_Player;
template <class T> class AttributePtr;
class Timer;

class Event_HackActivated;

//! Component handling hacks
/*
Must run between game component and physics component
*/

class HacksComponent : public IObserver
{
public:
	HacksComponent();
	~HacksComponent();

	virtual bool init();

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	//! A vector of size NROFHACKS with vectors that contains pairs for each active hack of that type.
	std::vector<std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>> activeHacks_; 
	//! Updates the hack for that player
	void updateHack(AttributePtr<Attribute_Player>& ptr_player, XKILL_Enums::HackType hackType);
	//! Handles an hackActivatedEvent and creates a timer/player pair.
	void handleHackActivatedEvent(Event_HackActivated* e);
	//! Checks if the hack should update. Used for hacks that should update under a certain condition, i.e. when a button is pressed.
	bool shouldUpdateTimer(AttributePtr<Attribute_Player>& ptr_player, XKILL_Enums::HackType hackType);
	void removeAllPlayerHacks(AttributePtr<Attribute_Player> playerAttribute);
	//! Removes the pair at index 'index' from the vector
	void removeIndexFromVector(std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>& vector, unsigned int index);

	void setPlayerAttributeHackFlags(AttributePtr<Attribute_Player> player, XKILL_Enums::HackType hacktype, bool truthValue);
};