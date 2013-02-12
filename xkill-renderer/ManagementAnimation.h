#ifndef XKILL_RENDERER_MANAGEMENT_ANIMATION_H
#define XKILL_RENDERER_MANAGEMENT_ANIMATION_H

#include <vector>

#include <xkill-utilities/SkinnedData.h>

#include "d3dInterface.h"


class ManagementAnimation : public D3DInterface
{
public:
	ManagementAnimation();
	virtual ~ManagementAnimation();
	virtual void reset();

	void addAnimation(SkinnedData* skinnedData);

	float time;

	SkinnedData* getAnimation(unsigned int index);

private:

	std::vector<SkinnedData*> animations_;
};

#endif //XKILL_RENDERER_MANAGEMENT_ANIMATION_H