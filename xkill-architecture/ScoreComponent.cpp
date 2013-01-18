#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>

ATTRIBUTES_DECLARE_ALL

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
}

ScoreComponent::~ScoreComponent()
{

}

bool ScoreComponent::init()
{
	for(unsigned int i = 0; itrPlayer.hasNext(); i++)
	{
		playerIndices.push_back(i);
		itrPlayer.getNext();
	}

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