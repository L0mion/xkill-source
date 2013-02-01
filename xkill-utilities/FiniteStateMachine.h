#pragma once

#include <vector>
#include <stack>
#include <map>

#include "FiniteState.h"
#include "Util.h"

/// Generic Finite State Machine implementation with stack and event handling.
/** \ingroup xkill-utilities
The fact that FiniteStateMachine (FSM) derives from FiniteState makes it possible to construct hierarchical FSMs with nested machines. 
However each such machine requires its own transition rules to operate. Please derive from FiniteStateMachine to provide these rules.
Specificly the methods CheckTransitions(), Enter(), Exit() and possibly Update(float) might need different implementations.
Suggestions follow:
- CheckTransitions() should provide rules for transitioning to other states (parent or siblings).
- Either Enter() should set a suitable starting state (for example the default state), or
- Exit() should make sure the FSM is ready for use the next time it's needed (for example resetting it to the default state)
- Update() might need to do some extra supporting calculations that makes the job in  CheckTransitions() easier.

Example implementations of Enter() and Exit() relying on default state is provided.

@todo Add copy constructor & assignment operator
@todo Add deactivation mechanism to States & FSM
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
	void Update(float dt); ///< Checks for and handles transitions, updates current state
	/**< 
	Will return early if:
	- No states have been added to the machine
	- No valid state could be established (neither a current nor a default state exists)
	*/

	StateType CheckTransitions(bool& out_isReplacementState) const; ///< This FSM is not intended to be be nested. 
	/**< 
	Please derive from it to create a nestable FSM with specific rules. 
	@returns Always return the type of itself, making it impossible to transition away this machine if you attempt to nest it.*/
	
	void Enter(); ///< Enters currentState if set, otherwise attempts to use default.
	/**< Example implementation that can be used when deriving to create nestable FSMs. */

	void Exit(); ///< Exits currentState if set, then attempts to set currentState to default.
	/**< Example implementation that can be used when deriving to create nestable FSMs. */

	void Reset(); ///< Exits current states, empties currentState stack & recursively resets all added states. If a default state exists it will be used as the new current state.
	
	void Nuke(); ///< Exits current states, empties currentState stack & deallocates all added states. Nested FSMs will be recursively nuked. Default state is set to null.
	
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
	bool HandleTransition(StateType goalStateType, bool replaceCurrentState); ///< Contains the necessary logic for performing a state transition.  
	/**< 
	Prints debug messages to the console should any problem arise
	@pre currentState stack must not be empty */

	virtual void handleEvent_StateChanged( Event_StateChanged* e);
	void handleEvent_SyncStateCommand( Event_SyncStateCommand* e );

	std::map<StateType, FiniteState*> states_; ///< All states currently associated with the State Machine.
	std::stack<FiniteState*> currentState_; ///< A stack containing the history of current states with the most recent on top.
	FiniteState* defaultState_; ///< Default fallback state
};