#pragma once

#include "dllUtilities.h"
#include "IObserver.h"

enum DLL_U StateType
{
	SPECIAL_STATE_NONE,
	SPECIAL_STATE_BACK,
	MACHINE_STATE_DEFAULT,
	STATE_DEATHMATCH,
	STATE_MAINMENU,
	STATE_GAMEOVER
};

/// FiniteState
/** 
Abstract class defining the interface of Finite States as well as som basic implementations
\ingroup xkill-utilities

@todo Add copy constructor & assignment operator
@todo Add deactivation mechanism to States & FSM
*/
class DLL_U FiniteState: public IObserver
{
public:
	
	virtual void Enter(); ///< Automatically called by a FiniteStateMachine upon entering the state (during state transition).
	/**< By default this method does nothing, override to provide state specific implementation.*/

	virtual void Update(float dt) = 0; ///< Method containing the majority of state specific logic.
	/**< 
	This method is where the majority of state specific logic will reside.
	It is intended to be called by a FiniteStateMachine each frame to update the state (given the state is current).*/
	
	virtual void Exit(); ///< Automatically called by a FiniteStateMachine upon exiting the state (during state transition)
	/**< By default this method does nothing, override to provide state specific implementation.*/

	virtual void Reset(); ///< Resets the state to its original or default setting. Is not supposed to perform memory deallocation.
	/**< By default this method does nothing, override to provide state specific implementation.*/

	virtual void Nuke(); ///< Same as Reset but also performs memory deallocation. Expect things to break horribly if you try to use the state after this!
	/**< By default this method does nothing, override to provide state specific implementation.*/
	
	virtual StateType CheckTransitions(bool& out_isReplacementState) const = 0; ///< Queries the state for possible state transition.
	/**< 
	Can be used by a FiniteStateMachine to poll the state each frame. 
	@returns If the state wants a transition to occur it returns the desired StateType. \n
			If no transition is desired the state use its own type as return value.
	@param[out] out_isReplacementState True indicates that the new state expect to replace the current (standard FSM behavior, the old state should be discarded). \n
			False indicates that the new state expects to be placed on a stack or similar, making it possible to use the BACK command at a later point in time.
	*/

	void onEvent(Event* e) = 0;
	void onUpdate(float delta); /**< Necessary to satisfy interface IObserver, delegates call to Update(float) */

	FiniteState(StateType type = SPECIAL_STATE_NONE); ///< @note Default constructor is generated using default parameters
	virtual ~FiniteState();

	StateType GetType() const;

protected:
	StateType type_; ///< Must be unique to every state
	//bool isActive_; 
};
