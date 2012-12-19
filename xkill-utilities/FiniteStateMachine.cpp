#include "FiniteStateMachine.h"
#include <iostream>
#include <iterator>


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

bool FiniteStateMachine::SetDefaultState( StateType state )
{
	bool wasSet = false;

	if(states_.count(state) > 0)
	{
		defaultState_ = states_[state];
		wasSet = true;
	}

	#if defined(DEBUG) | defined(_DEBUG)
	if (wasSet)
	{
		std::cout << "[SUCCESS] FiniteStateMachine::SetDefaultState | "
			<< "Default state successfully set to FiniteState with enum value " 
			<< state << "\n";
	}
	else
	{
		std::cout << "[FAIL] FiniteStateMachine::SetDefaultState | "
			<< "Attempt to set default state set to FiniteState with enum value " 
			<< state << " was unsuccessful. Did you remember to add it first? \n";
	}
	#endif

	return wasSet;
}

void FiniteStateMachine::Update( float dt )
{
	//no states to handle, nothing to do
	if (states_.size() == 0)
	{
		std::cout << "Warning: FiniteStateMachine::Update are being called on an empty machine" << std::endl;
		return;
	}

	//if no current state: attempt to set it to default
	if (currentState_.empty() && defaultState_)
	{
		currentState_.push(defaultState_);
	}

	//no valid  state could be established, nothing to do
	if (currentState_.empty())
	{
		std::cout << "Warning: FiniteStateMachine::Update, current state is invalid" << std::endl;
		return;
	}

	HandleTransitions();

	currentState_.top()->Update(dt);
}

StateType FiniteStateMachine::CheckTransitions( bool& outShouldReplaceCurrent ) const
{
	return type_; //returning my own StateType means no transition will take place if I am the current state
}

void FiniteStateMachine::Exit()
{
	if (currentState_.top())
	{
		currentState_.top()->Exit();
	}
}

void FiniteStateMachine::Reset()
{
	Exit();

	//empty stack of current states
	while (!currentState_.empty())
	{
		currentState_.pop();
	}

	//reset all added states
	for (auto pair : states_)
	{
		pair.second->Reset();
	}

	if (defaultState_)
	{
		currentState_.push(defaultState_);
		currentState_.top()->Enter();
	}
}

void FiniteStateMachine::Nuke()
{
	Exit();
	
	//empty stack of current states
	while (!currentState_.empty())
	{
		currentState_.pop();
	}

	//deallocate all states and clear map
	for (std::map<StateType, FiniteState*>::iterator it = states_.begin(); 
		it != states_.end(); 
		it++)
	{
		delete it->second; //what if the state is another state machine?! will result in memory leak!
		//states_.erase(it); //alternative solution is to call states_clear() after the loop instead
	}
	states_.clear();

	defaultState_ = 0; //all states removed, no valid default state exists
}

FiniteStateMachine::FiniteStateMachine( StateType type, FiniteState* defaultAndCurrentState ):
	FiniteState(type),
	defaultState_(0)
{
	if (defaultAndCurrentState)
	{
		AddState(defaultAndCurrentState);
		defaultState_ = defaultAndCurrentState;
		currentState_.push(defaultAndCurrentState); 
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
	//States are injected and memory deallocation is therefore handled by caller (See Wikipedia RAII)
	//Deallocation can however be performed by a specific call to the method Nuke()
}

void FiniteStateMachine::HandleTransitions()
{
	//check for transitions
	bool shouldReplaceCurrentState = true;
	StateType currentStateType = currentState_.top()->GetType();
	StateType goalStateType = currentState_.top()->CheckTransitions(shouldReplaceCurrentState);

	//switch if a transition is both possible and neccessary
	bool stateHasChanged = goalStateType != currentStateType;
	bool goalStateExists = states_.count(goalStateType) > 0;
	if (stateHasChanged && goalStateExists)
	{ 
		if (shouldReplaceCurrentState)
		{	//replace stack top
			currentState_.top()->Exit();
			currentState_.top() = states_[goalStateType];
		}
		else //add to stack
		{
			currentState_.push(states_[goalStateType]);
		}
		currentState_.top()->Enter();
	}
	else if (goalStateType == SPECIAL_STATE_BACK && !currentState_.empty())
	{ //handle special transition
		currentState_.pop();
		if (currentState_.empty())
		{
			currentState_.push(defaultState_);
		}
	}
	else //requested transition could not be performed
	{
		//TODO: Print warning on trying to transition to non added state
	}
}
