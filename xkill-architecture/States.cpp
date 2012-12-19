#include "States.h"

DeathMatchState::DeathMatchState(StateType type) : FiniteState(type)
{
}
DeathMatchState::~DeathMatchState()
{

}

void DeathMatchState::Update(float dt)
{

}

StateType DeathMatchState::CheckTransitions(bool& outShouldReplaceCurrent) const
{
	return SPECIAL_STATE_NONE;//check
}


MainMenuState::MainMenuState(StateType type)
{
}
MainMenuState::~MainMenuState()
{
}

void MainMenuState::Update(float dt)
{
}
StateType MainMenuState::CheckTransitions(bool& outShouldReplaceCurrent) const
{
	return SPECIAL_STATE_NONE;//check
}