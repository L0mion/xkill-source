#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>

ATTRIBUTES_DECLARE_ALL

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);

	schedulerTime_ = 5.0f;
	currentSchedulerTime_ = schedulerTime_;
}

ScoreComponent::~ScoreComponent()
{

}

bool ScoreComponent::init()
{
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
	default:
		break;
	}
}

void ScoreComponent::onUpdate(float delta)
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		currentSchedulerTime_--;
		if(currentSchedulerTime_ >= 0.0f)
		{
			currentSchedulerTime_ = schedulerTime_;

			int topPlayerIndex = -1;
			int topPriority = 0;

			while(itrPlayer.hasNext())
			{
				AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

				if(ptr_player->priority > 0 && ptr_player->priority > topPriority)
				{
					topPlayerIndex = itrPlayer.storageIndex();
					topPriority = ptr_player->priority;
					ptr_player->priority = 0;
				}
			}

			if(topPlayerIndex >= 0)
				itrPlayer.at(topPlayerIndex)->totalExecutionTime++;
		}
	}
}

void ScoreComponent::sort(std::vector<int>& elements)
{
	bool alreadySorted = true;

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		alreadySorted = true;

		for(unsigned int j = 0; j < (elements.size() - 1); j++)
		{
			if(itrPlayer.at(elements[j])->priority > itrPlayer.at(elements[j + 1])->priority )
			{
				swap(elements, j, j + 1);
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