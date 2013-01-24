#ifndef XKILL_RENDERER_MANAGEMENTSPRITES_H
#define XKILL_RENDERER_MANAGEMENTSPRITES_H

#include "d3dInterface.h"

class ManagementSprites : public D3DInterface
{
public:

	ManagmentSprites();
	virtual ~ManagementSprites();

	virtual void reset();
private:
};

#endif // XKILL_RENDERER_MANAGEMENTSPRITES_H