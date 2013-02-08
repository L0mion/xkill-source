#include "Interface.h"
#include <iostream>

#include "PhysicsComponent.h"

static PhysicsComponent pc;

bool iinit()
{
	return pc.init();
}
void ionUpdate(float delta)
{
	pc.onUpdate(delta);
}
void ionEvent(Event* e)
{
	pc.onEvent(e);
}