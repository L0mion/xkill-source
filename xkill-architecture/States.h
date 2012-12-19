#pragma once

#include "dllArchitecture.h"
#include <xkill-utilities/FiniteState.h>

class DLL_A DeathMatchState : public FiniteState
{
public:
	DeathMatchState(StateType type);
	~DeathMatchState();

	void Update(float dt);
	StateType CheckTransitions(bool& outShouldReplaceCurrent) const;
};

class DLL_A MainMenuState : public FiniteState
{
public:
	MainMenuState(StateType type);
	~MainMenuState();

	void Update(float dt);
	StateType CheckTransitions(bool& outShouldReplaceCurrent) const;
};