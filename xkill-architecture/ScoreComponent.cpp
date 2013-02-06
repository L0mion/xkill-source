#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>

ATTRIBUTES_DECLARE_ALL

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);

	executingPlayer_ = nullptr;
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

	executingPlayer_ = nullptr;

	for(unsigned int i = 0; itrPlayer.hasNext(); i++)
	{
		playerIndices_.push_back(i);
		itrPlayer.getNext();
	}

	sort(playerIndices_);

	return true;
}

void ScoreComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_PLAYERDEATH:
		sort(playerIndices_);
		break;
	case EVENT_START_DEATHMATCH:
		init();
		break;
	default:
		break;
	}
}

void ScoreComponent::onUpdate(float delta)
{
	//if(GET_STATE() == STATE_DEATHMATCH)
	//{
		if(executionMode_)
		{
			currentCycleTime_ -= delta;
			if(currentCycleTime_ <= 0.0f)
			{
				if(executingPlayer_ == nullptr)	// Shouldn't happen, but if it does then leave execution mode
				{
					DEBUGPRINT("Score component: Executing player doesn't exist.");
					executionMode_ = false;
				}
				else
				{
					if(executingPlayer_->priority > 0)  // The player still has some priority so give it execution time
					{
						executingPlayer_->priority--;
						executingPlayer_->totalExecutionTime++;
					}
					else								// The player doesn't have any priority left so leave execution mode
					{
						executionMode_ = false;
						executingPlayer_ = nullptr;
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
					Attribute_Player* player = itrPlayer.getNext();

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
				}
				else if(topPriorityIsTied)	// Two or more players are tied for the ammount of priority
				{
					// Do nothing, wait until a single player is in the lead
				}
				else						// Execute the player with highest priority
				{
					executingPlayer_ = itrPlayer.at(topPlayerIndex);
					currentCycleTime_ = cycleTime_;
					executionMode_ = true;
					// Send event to notify other components that we're entering execution mode
				}
			}
		}
	//}
}

void ScoreComponent::sort(std::vector<int>& elements)
{
	bool alreadySorted = true;

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		alreadySorted = true;

		for(unsigned int j = 0; j < (elements.size() - 1); j++)
		{
			if(compareAttributes(itrPlayer.at(elements[i]), itrPlayer.at(elements[j + 1])))
			{
				swap(elements, i, j + 1);
				alreadySorted = false;
			}
		}

		if(alreadySorted)
			break;
	}
}

void ScoreComponent::swap(std::vector<int>& elements, int first, int second)
{
	int temp = elements[first];
	elements[first] = elements[second];
	elements[second] = temp;
}

bool ScoreComponent::compareAttributes(Attribute_Player* player1, Attribute_Player* player2)
{
	bool larger = false;

	if(player1->totalExecutionTime < player2->totalExecutionTime)
	{
		larger = true;
	}
	else if(player1->totalExecutionTime == player2->totalExecutionTime)
	{
		if(player1->priority < player2->priority)
		{
			larger = true;
		}
	}
	
	return larger;
}