#pragma once

#include "dllUtilities.h"
#include "IObserver.h"

enum DLL_U StateType
{
	SPECIAL_STATE_NONE,
	SPECIAL_STATE_BACK,
	MACHINE_STATE_DEFAULT,
	STATE_DEATHMATCH,
	STATE_MAINMENU
};

class DLL_U FiniteState: public IObserver
{
public:
	virtual void Enter();
	virtual void Update(float dt) = 0;
	virtual void Exit();
	virtual void Reset();
	virtual void Nuke();
	virtual StateType CheckTransitions(bool& out_isReplacementState) const = 0;
	StateType GetType() const;

	void onEvent(Event* e) = 0;
	void onUpdate(float delta);

	FiniteState(StateType type = SPECIAL_STATE_NONE); //Note: default constructor is generated using default parameters
	virtual ~FiniteState();

	//TODO: Add copy constructor & assignment operator
protected:
	StateType type_; //must be unique to every state
	//bool isActive_; //TODO: Add deactivation mechanism to States & FSM
};
