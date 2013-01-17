#include "FiniteState.h"

StateType FiniteState::GetType() const
{
	return type_;
}

void FiniteState::Enter()
{
	//Intentionally left blank
}

void FiniteState::Exit()
{
	//Intentionally left blank
}

void FiniteState::Reset()
{
	//Intentionally left blank
}

void FiniteState::Nuke()
{
	//Intentionally left blank
}

FiniteState::FiniteState( StateType type ):
	type_(type)
{
	//Intentionally left blank, see member initialization list 
}

FiniteState::~FiniteState()
{
	//Intentionally left blank
}

void FiniteState::onUpdate( float delta )
{
	Update(delta);
}

//void FiniteState::Update( float dt )
//{
//
//}

//StateType FiniteState::CheckTransitions( bool& outShouldReplaceCurrent ) const
//{
//	return SPECIAL_STATE_NONE;
//}

