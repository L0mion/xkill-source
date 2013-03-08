#include "HacksComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/Timer.h>
#include <xkill-utilities/Converter.h>

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

ATTRIBUTES_DECLARE_ALL

HacksComponent::HacksComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_HACK_ACTIVATED);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
}

HacksComponent::~HacksComponent()
{
	for(unsigned int i = 0; i < XKILL_Enums::HackType::NROFHACKTYPES; i++)
	{
		std::vector<std::pair<Timer*, AttributePtr<Attribute_Player>>*>& vector = activeHacks_[i];

		for(unsigned int j = 0; j < vector.size(); j++)
		{
			SAFE_DELETE(vector[j]->first);
			SAFE_DELETE(vector[j]);
		}
	}
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
		{
			handleHackActivatedEvent(static_cast<Event_HackActivated*>(e));
			break;
		}
	case EVENT_PLAYERDEATH:
		{
			Event_PlayerDeath* event_PlayerDeath = static_cast<Event_PlayerDeath*>(e);
			removeAllPlayerHacks(itrPlayer.at(event_PlayerDeath->playerAttributeIndex));
		}
		break;
	default:
		{
			break;
		}
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

			if(shouldUpdateTimer(activeHacks_[i][j]->second, static_cast<XKILL_Enums::HackType>(i)))
				timer->update(delta);

			if(timer->hasTimerExpired())
			{
				setPlayerAttributeHackFlags(activeHacks_[i][j]->second, static_cast<XKILL_Enums::HackType>(i), false);
				removeIndexFromVector(activeHacks_[i], j);

				SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_JETPACK, itrPlayer.ownerIdAt(activeHacks_[i][j]->second.index())));

				DEBUGPRINT("Hack " << Converter::IntToStr(i) << " expired.");
			}
			else
			{
				updateHack(activeHacks_[i][j]->second, static_cast<XKILL_Enums::HackType>(i));
			}
		}
	}
}
	
void HacksComponent::updateHack(AttributePtr<Attribute_Player>& ptr_player, XKILL_Enums::HackType hackType)
{
	switch(hackType)
	{
	case XKILL_Enums::HackType::SPEEDHACK:
		ptr_player->currentSpeed *= 1.5f;
		break;
	case XKILL_Enums::HackType::JETHACK:
		if(ptr_player->ptr_input->jetpack)
			ptr_player->jetpack = true;
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
			if(activeHacks_[e->hackType][i]->first->getTimeLeft() < e->time)
			{
				activeHacks_[e->hackType][i]->first->setStartTime(e->time);
				activeHacks_[e->hackType][i]->first->resetTimer();

				DEBUGPRINT("Existing hack " << Converter::IntToStr(e->hackType) << " #" << Converter::IntToStr(i) << " was reset with value " << Converter::FloatToStr(e->time));
			}
			existed = true;

			break;
		}
	}

	if(!existed)
	{
		Timer* timer = new Timer(e->time); //Convert from s to ms
		activeHacks_[e->hackType].push_back(new std::pair<Timer*, AttributePtr<Attribute_Player>>(timer, e->player));
		setPlayerAttributeHackFlags(e->player, e->hackType, true);

		DEBUGPRINT("Player picked up hack " << Converter::IntToStr(e->hackType) << " with value " << Converter::FloatToStr(e->time));
	}
}

bool HacksComponent::shouldUpdateTimer(AttributePtr<Attribute_Player>& ptr_player, XKILL_Enums::HackType hackType)
{
	bool updateTimer = false;

	switch(hackType)
	{
	case XKILL_Enums::HackType::JETHACK:
		if(ptr_player->ptr_input->jetpack)
		{
			updateTimer = true;
		}
		break;
	case XKILL_Enums::HackType::SPEEDHACK:
	default:
		updateTimer = true;
		break;
	}

	return updateTimer;
}

void HacksComponent::removeAllPlayerHacks(AttributePtr<Attribute_Player> playerAttribute)
{
	for(unsigned int i = 0; i < XKILL_Enums::HackType::NROFHACKTYPES; i++)
	{
		for(unsigned int j = 0; j < activeHacks_[i].size(); j++)
		{
			if(playerAttribute == activeHacks_[i][j]->second)
			{
				//setPlayerAttributeHackFlags(activeHacks_[i][j]->second, static_cast<XKILL_Enums::HackType>(i), false);
				removeIndexFromVector(activeHacks_[i], j);
			}
		}
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
void HacksComponent::setPlayerAttributeHackFlags(AttributePtr<Attribute_Player> player, XKILL_Enums::HackType hacktype, bool truthValue)
{
	switch(hacktype)
	{
	case XKILL_Enums::HackType::JETHACK:
		player->jetHackActive = truthValue;
		break;
	case XKILL_Enums::HackType::SPEEDHACK:
		player->speedHackActive = truthValue;
		break;
	case XKILL_Enums::HackType::CYCLEHACK:
		player->cycleHackActive = truthValue;
		break;
	}
}