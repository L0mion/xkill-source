#pragma once

#include "EventManager.h"
#include "AttributeManager.h"
#include <iostream>
using namespace std;

//
// Components execute stuff in a orderly fashion
//

// Parent

class IComponent : public IObserver
{
public:
	virtual ~IComponent(){};
	virtual void onUpdate(float delta) = 0;
	virtual void onEvent(Event* e) = 0;
};