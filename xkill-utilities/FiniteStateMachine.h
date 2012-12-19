#pragma once

#include <vector>
#include <stack>
#include <map>

#include "dllUtilities.h"
#include "FiniteState.h"

//! Finite State Machine
/*!

\ingroup xkill-utilities
*/
class DLL_U FiniteStateMachine : public FiniteState
{
public:
	bool AddState(FiniteState* state);
	bool SetDefaultState(StateType state);
	
	//Overload of derived methods
	StateType CheckTransitions(bool& outShouldReplaceCurrent) const;
	void Update(float dt);
	void Exit();
	void Reset();
	void Nuke();

	//Constructors and Destructors
	FiniteStateMachine
	(
		StateType type = MACHINE_STATE_DEFAULT, 
		FiniteState* defaultAndCurrentState = NULL); //Note: default constructor is generated using default parameters
	~FiniteStateMachine(); //States are injected and memory deallocation is therefore handled by caller (See Wikipedia RAII)

	//TODO: Add copy constructor & assignment operator
protected:
	void HandleTransitions();

	std::map<StateType, FiniteState*> states_;
	std::stack<FiniteState*> currentState_;
	FiniteState* defaultState_;
};