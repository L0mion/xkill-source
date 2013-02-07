#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>

ATTRIBUTES_DECLARE_ALL

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);

	executingPlayerIndex_ = -1;
}

ScoreComponent::~ScoreComponent()
{

}

bool ScoreComponent::init()
{
	schedulerTime_ = 30.0f;
	currentSchedulerTime_ = schedulerTime_;

	cycleTime_ = 1.0f;
	currentCycleTime_ = cycleTime_;	

	executionMode_ = false;
	executingPlayerIndex_ = -1;

	victoryScore_ = 20;

	return true;
}

void ScoreComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_START_DEATHMATCH:
		init();
		break;
	default:
		break;
	}
}

void ScoreComponent::onUpdate(float delta)
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		//if scheduler game mode
		schedulerScoreCounting(delta);

		//if death match game mode
		//deathMatchScoreCounting(delta)
	}
}

void ScoreComponent::schedulerScoreCounting(float delta)
{
	if(executionMode_)
	{
		currentCycleTime_ -= delta;
		if(currentCycleTime_ <= 0.0f)
		{
			currentCycleTime_ = cycleTime_;

			if(executingPlayerIndex_ == -1)	// Shouldn't happen, but if it does then leave execution mode
			{
				DEBUGPRINT("Score component: Executing player doesn't exist.");
				executionMode_ = false;
			}
			else
			{
				AttributePtr<Attribute_Player> executingPlayer = itrPlayer.at(executingPlayerIndex_);

				if(executingPlayer->priority > 0)  // The player still has some priority so give it execution time
				{
					executingPlayer->priority--;
					executingPlayer->totalExecutionTime++;
				}
				else								// The player doesn't have any priority left so leave execution mode
				{
					executionMode_ = false;
					executingPlayerIndex_ = -1;
					currentSchedulerTime_ = schedulerTime_;
					// Send event to notify other components that we're leaving execution mode
				}
			}
		}
	}
	else
	{
		currentSchedulerTime_ -= delta;
		if(currentSchedulerTime_ <= 0.0f)
		{
			int topPlayerIndex = -1;
			int topPriority = 0;
			bool topPriorityIsTied = false;

			while(itrPlayer.hasNext())	// Loop through all player and find if anyone has top priority
			{
				//Attribute_Player* player = itrPlayer.getNext();
				AttributePtr<Attribute_Player> player = itrPlayer.getNext();

				if(player->priority > 0)
				{
					if(player->priority > topPriority)		// Current player had higher priority than last top player
					{
						topPlayerIndex = itrPlayer.storageIndex();
						topPriority = player->priority;
						topPriorityIsTied = false;
					}
					else if(player->priority == topPriority)	// Current player had the same priority as last top player
					{
						topPriorityIsTied = true;
					}
				}
			}

			if(topPlayerIndex == -1)	// All players had zero priority
			{
				// Punish them all
				currentSchedulerTime_ = schedulerTime_;
			}
			else if(topPriorityIsTied)	// Two or more players are tied for the ammount of priority
			{
				// Do nothing, wait until a single player is in the lead
			}
			else						// Execute the player with highest priority
			{
				executingPlayerIndex_ = topPlayerIndex;
				currentCycleTime_ = cycleTime_;
				executionMode_ = true;
				// Send event to notify other components that we're entering execution mode
			}
		}
	}

	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->totalExecutionTime >= victoryScore_)
		{
			SEND_EVENT(&Event(EVENT_GAME_OVER));
		}
	}
}

void ScoreComponent::deathMatchScoreCounting(float delta)
{
	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->priority >= victoryScore_)
		{
			SEND_EVENT(&Event(EVENT_GAME_OVER));
		}
	}
}
