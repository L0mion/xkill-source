#pragma once

#include "IObserver.h"
#include "dllUtilities.h"

class DLL IComponent : public IObserver
{
public:
	virtual ~IComponent(){};
	virtual void onUpdate(float delta) = 0;
	virtual void onEvent(Event* e) = 0;
};