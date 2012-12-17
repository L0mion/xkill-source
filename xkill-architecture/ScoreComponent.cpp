#include "ScoreComponent.h"

#include <xkill-architecture/AttributeManager.h>
#include <xkill-utilities/EventType.h>

ScoreComponent::ScoreComponent()
{
	playerAttributes_ = nullptr;
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
}

ScoreComponent::~ScoreComponent()
{

}

bool ScoreComponent::init(std::vector<PlayerAttribute>* playerAttribute)
{
	playerAttributes_ = playerAttribute;

	for(unsigned int i = 0; i < playerAttributes_->size(); i++)
		playerIndices.push_back(i);

	sort(playerIndices);

	return true;
}

void ScoreComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_PLAYERDEATH:
		sort(playerIndices);
		break;
	default:
		break;
	}
}

void ScoreComponent::onUpdate(float delta)
{

}

void ScoreComponent::sort(std::vector<int>& elements)
{
	bool alreadySorted = true;

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		alreadySorted = true;

		for(unsigned int j = 0; j < (elements.size() - 1); j++)
		{
			if(playerAttributes_->at(elements[j]).priority > playerAttributes_->at(elements[j + 1]).priority )
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