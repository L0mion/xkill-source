#pragma once

#include <vector>
#include <stack>
#include <map>

#include "dllUtilities.h"
#include "FiniteState.h"
#include "EventManager.h"

/// Generic Finite State Machine implementation with stack and event handling.
/** \ingroup xkill-utilities
The fact that the FiniteStateMachine (FSM) derives from FiniteState makes it possible to construct hierarchical FSMs with nested machines.
@todo Add copy constructor & assignment operator
*/
class DLL_U FiniteStateMachine : public FiniteState
{
public:
	bool AddState(FiniteState* state); ///< Adds an already allocated FiniteState to the state machine (Dependency Injection).
	/**< 
	@param state A FiniteState with a unique StateType that have not been previously added. It must not be NULL.
	@returns True indicates operation success and false indicates failure (the state has already been added or state param is null).*/

	bool SetDefaultState(StateType state); ///< Changes the default (fallback) state to the requested type.
	/**< 
	@pre Assumes that a finite state with the requested StateType has been added.
	@param state A StateType of an already added FiniteState
	@returns True if operation success. False if default state could not be changed*/
	
	//////////////////////////////////////////////////////////////////////////
	// Overridden methods
	//////////////////////////////////////////////////////////////////////////
	StateType CheckTransitions(bool& out_isReplacementState) const;

	void Update(float dt); ///< Checks for and handles transitions, updates current state
	/**< 
	Will return early if:
	- No states have been added to the machine
	- No valid state could be established (neither a current nor a default state exists)
	*/
	
	void Exit(); ///< Calls Exit on current state
	
	void Reset(); ///< Calls Exit(), empties currentState stack & recursively resets all added states. If a default state exists it will be used as the new current state.
	
	void Nuke(); ///< Calls Exit(), empties currentState stack & deallocates all added states. Nested FSMs will be recursively nuked. Default state is set to null.
	
	void onEvent(Event* e); ///< Receives and handles selected events.
	/**< If current state is a nested FiniteStateMachine the event is passed on and ignored. 
	*/

	//////////////////////////////////////////////////////////////////////////
	// Constructors and Destructors
	//////////////////////////////////////////////////////////////////////////
	FiniteStateMachine(
		StateType type = MACHINE_STATE_DEFAULT, 
		FiniteState* defaultAndCurrentState = NULL 
		);
	/**<
	@param type Type identifier for the state machine. 
	Please supply unique type names for each state machine usage, especially if planning to use FSM event handling features.
	@param defaultAndCurrentState The supplied state will be used both as default and starting current state. 
	No additional calls to AddState(FiniteState*) or SetDefaultState(StateType) will be necessary for this state.
	@note Default constructor is generated from default parameters */
	
	~FiniteStateMachine(); ///< @note States are injected and their memory deallocation is therefore handled by caller (See Wikipedia RAII) @see Nuke(), AddState(FiniteState*)

protected:
	void HandleTransition(StateType goalStateType, bool replaceCurrentState); ///< Contains the necessary logic for performing a state transition.  
	/**< Prints debug messages to the console should any problem arise
	@pre currentState_ stack must not be empty */

	virtual void handleEvent_StateChanged( Event_StateChanged* e);

	std::map<StateType, FiniteState*> states_; ///< All states currently associated with the State Machine.
	std::stack<FiniteState*> currentState_; ///< A stack of states with the current (most recent) on top.
	FiniteState* defaultState_; ///< Default fallback state
};