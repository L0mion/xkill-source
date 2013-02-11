#include "HacksComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/Timer.h>

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

ATTRIBUTES_DECLARE_ALL

HacksComponent::HacksComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_HACK_ACTIVATED);
}

HacksComponent::~HacksComponent()
{

}

bool HacksComponent::init()
{
	for(int i = 0; i < XKILL_Enums::HackType::NROFHACKTYPES; i++)
	{
		activeHacks_.push_back(std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>());
	}

	return true;
}

void HacksComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_HACK_ACTIVATED:
		handleHackActivatedEvent(static_cast<Event_HackActivated*>(e));
		break;
	default:
		break;
	}
}

void HacksComponent::onUpdate(float delta)
{
	Timer* timer;

	for(unsigned int i = 0; i < XKILL_Enums::NROFHACKTYPES; i++)
	{
		for(unsigned int j = 0; j < activeHacks_[i].size(); j++)
		{
			timer = activeHacks_[i][j]->first;

			timer->update(delta);
			if(timer->hasTimerExpired())
			{
				removeIndexFromVector(activeHacks_[i], i);
			}
			else
			{
				activeHacks_[i][j]->second->currentSpeed *= 1.5f;
			}
		}
	}
}
	
void HacksComponent::handleHack(AttributePtr<Attribute_Player>& player, XKILL_Enums::HackType hackType)
{
	switch(hackType)
	{
	case XKILL_Enums::HackType::SPEEDHACK:
		player->currentSpeed *= 1.5f;
		break;
	case XKILL_Enums::HackType::JETHACK:
		
		break;
	default:
		break;
	}
}

void HacksComponent::handleHackActivatedEvent(Event_HackActivated* e)
{
	bool existed = false;

	for(unsigned int i = 0; i < activeHacks_[e->hackType].size(); i++)
	{
		if(e->player == activeHacks_[e->hackType][i]->second)
		{
			activeHacks_[e->hackType][i]->first->setStartTime(e->time);
			activeHacks_[e->hackType][i]->first->resetTimer();
			existed = true;
			break;
		}
	}

	if(!existed)
	{
		Timer* timer = new Timer(e->time);
		activeHacks_[e->hackType].push_back(new std::pair<Timer*, AttributePtr<Attribute_Player>>(timer, e->player));
	}
}

void HacksComponent::removeIndexFromVector(std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>& vector, unsigned int index)
{
	if(vector.size() <= 0)
		return;

	std::pair<Timer*, AttributePtr<Attribute_Player>>* temp = vector[index];
	vector[index] = vector[vector.size() - 1];
	vector[vector.size() - 1] = temp;

	SAFE_DELETE(vector[vector.size() - 1]->first);
	SAFE_DELETE(vector[vector.size() - 1]);
	vector.pop_back();
}