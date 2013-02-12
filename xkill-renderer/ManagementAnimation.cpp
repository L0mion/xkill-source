
#include "ManagementAnimation.h"
#include "renderingUtilities.h"

ManagementAnimation::ManagementAnimation()
{
	time = 0.0f;
}
ManagementAnimation::~ManagementAnimation()
{
	for(unsigned int i=0; i<animations_.size(); i++)
		SAFE_DELETE(animations_[i]);
}

void ManagementAnimation::reset()
{
}

SkinnedData* ManagementAnimation::getAnimation(unsigned int index)
{
	time += 0.002f;
	if(time > 1.0f)
		time = 0.0f;

	SkinnedData* skinnedData = nullptr;
	if(index < animations_.size())
		skinnedData = animations_.at(index);

	return skinnedData;
}

void ManagementAnimation::addAnimation(SkinnedData* skinnedData)
{
	animations_.push_back(skinnedData);
}