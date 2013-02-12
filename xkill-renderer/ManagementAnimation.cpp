
#include "ManagementAnimation.h"
#include "renderingUtilities.h"

ManagementAnimation::ManagementAnimation()
{
}
ManagementAnimation::~ManagementAnimation()
{
	for(unsigned int i=0; i<animations_.size(); i++)
		SAFE_DELETE(animations_[i]);
}

void ManagementAnimation::reset()
{
}

void ManagementAnimation::addAnimation(SkinnedData* skinnedData)
{
	animations_.push_back(skinnedData);
}