#include "FiniteStateMachine.h"
#include <iostream>



bool FiniteStateMachine::AddState( FiniteState* state )
{
	auto result = states_.insert(
		std::pair<StateType, FiniteState*>(state->GetType(), state));
	
	#if defined(DEBUG) | defined(_DEBUG)
	if(result.second)
	{
		std::cout << "[SUCCESS] FiniteStateMachine::AddState | "
			<< "Adding FiniteState with enum value " 
			<< state->GetType() 
			<< " succeded\n";
	}
	else
	{
		std::cout << "[FAIL] FiniteStateMachine::AddState | "
			<< "Adding FiniteState with enum value " 
			<< state->GetType() 
			<< " was unsuccessful, did you add it twice? " 
			<< "Note: A particular state may only be added once per state machine\n";
	}
	#endif

	return result.second;
}

bool FiniteStateMachine::SetDefaultState( FiniteState* state )
{
	bool wasSet = false;

	if(states_.count(state->GetType()) > 0)
	{
		defaultState_ = state;
		wasSet = true;
	}

	#if defined(DEBUG) | defined(_DEBUG)
	if (wasSet)
	{
		std::cout << "[SUCCESS] FiniteStateMachine::SetDefaultState | "
			<< "Default state successfully set to FiniteState with enum value " 
			<< state->GetType() << "\n";
	}
	else
	{
		std::cout << "[FAIL] FiniteStateMachine::SetDefaultState | "
			<< "Attempt to set default state set to FiniteState with enum value " 
			<< state->GetType() << " was unsuccessful. Did you remember to add it first? \n";
	}
	#endif

	return wasSet;
}

void FiniteStateMachine::Update( float dt )
{
	//no states to handle, nothing to do
	if (states_.size() == 0)
		return; //TODO Print warning about update being called on empty machine

	//if no current state attempt to set it to default
	if (!currentState_ && defaultState_)
	{
		currentState_ = defaultState_;
	}
	//no valid  state could be established, nothing to do
	if (!currentState_)
	{
		return; //TODO Print warning on state being invalid
	}

	//check for transitions
	bool shouldReplaceCurrentState = true;
	StateType currentStateType = currentState_->GetType();
	StateType goalStateType = currentState_->CheckTransitions(shouldReplaceCurrentState);

	//switch if there was a transition
	if (goalStateType != currentStateType)
	{
		//TODO: Make changes to this code when adding Stack to FSM
		if (shouldReplaceCurrentState)
		{
			currentState_->Exit();
			currentState_ = states_[goalStateType];
			currentState_->Enter();
		}
		else
		{
			currentState_->Exit();
			currentState_ = states_[goalStateType];
			currentState_->Enter();
		}
		currentState_->Update(dt);
	}
}

StateType FiniteStateMachine::CheckTransitions( bool& outShouldReplaceCurrent ) const
{
	
	return type_; //returning my own StateType means no transition will take place if I am the current state
}

void FiniteStateMachine::Reset()
{
	Exit();
	if (currentState_)
	{
		currentState_->Exit();
	}
	currentState_ = defaultState_;

	//reset all added states
	for (auto pair : states_)
	{
		pair.second->Reset();
	}

	if (defaultState_)
	{
		defaultState_->Enter();
	}
}

FiniteStateMachine::FiniteStateMachine( StateType type, FiniteState* defaultState ):
	FiniteState(type),
	defaultState_(0),
	currentState_(0)
{
	if (defaultState)
	{
		AddState(defaultState);
		SetDefaultState(defaultState);
		currentState_ = defaultState; 
	}
}


FiniteStateMachine::~FiniteStateMachine()
{
	//States are injected and memory deallocation is therefore handled by caller (See Wikipedia RAII)
}
