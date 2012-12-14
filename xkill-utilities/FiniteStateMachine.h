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
	bool SetDefaultState(FiniteState* state);
	
	//Overload of derived methods
	StateType CheckTransitions(bool& outShouldReplaceCurrent) const;
	void Update(float dt);
	void Reset();

	//Constructors and Destructors
	FiniteStateMachine
	(
		StateType type = MACHINE_STATE_DEFAULT, 
		FiniteState* defaultState = NULL
	); //Note: default constructor is generated using default parameters
	~FiniteStateMachine(); //States are injected and memory deallocation is therefore handled by caller (See Wikipedia RAII)

	//TODO: Add copy constructor & assignment operator
protected:
	std::map<StateType, FiniteState*> states_;
	FiniteState* currentState_;
	FiniteState* defaultState_;
};