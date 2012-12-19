#pragma once

#include "dllUtilities.h"

enum DLL_U StateType
{
	SPECIAL_STATE_NONE,
	SPECIAL_STATE_BACK,
	MACHINE_STATE_DEFAULT,
	STATE_DEATHMATCH,
	STATE_MAINMENU
	//FINITE_STATE_YOURSTATEHERE
};

class DLL_U FiniteState
{
public:
	virtual void Enter();
	virtual void Update(float dt) = 0;
	virtual void Exit();
	virtual void Reset();
	virtual void Nuke();
	virtual StateType CheckTransitions(bool& outShouldReplaceCurrent) const = 0;
	StateType GetType() const;

	FiniteState(StateType type = SPECIAL_STATE_NONE); //Note: default constructor is generated using default parameters
	virtual ~FiniteState();

	//TODO: Add copy constructor & assignment operator
protected:
	StateType type_; //must be unique to every state
	//bool isActive_; //TODO: Add deactivation mechanism to States & FSM
};
