#include "ScoreComponent.h"

ScoreComponent::ScoreComponent()
{

}

ScoreComponent::~ScoreComponent()
{

}

void ScoreComponent::onEvent(Event* e)
{

}

void ScoreComponent::onUpdate(float delta)
{

}

void ScoreComponent::sort(int* elements, int nrOfElements)
{
	if(nrOfElements <= 1)
		return;

	int leftSize = nrOfElements / 2;
	int rightSize = (nrOfElements - leftSize);

	int* left = &elements[0];
	int* right = &elements[leftSize];

	sort(left, leftSize);
	sort(right, rightSize);

	int leftIndex = 0;
	int rightIndex = 0;

	for(; (leftIndex < leftSize) && (rightIndex < rightSize);)
	{
		if(left[leftIndex] > right[rightIndex])
		{

		}
	}
}

void ScoreComponent::swap(int* elements, int first, int second)
{
	int temp = elements[first];
	elements[first] = elements[second];
	elements[second] = temp;
}