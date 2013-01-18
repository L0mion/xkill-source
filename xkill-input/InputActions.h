#pragma once

#include <string>

struct InputAction
{
	InputAction()
	{
		initializeStringArray();
	}

	//Remember to update string array if changes to the enum has been made
	enum InputActionEnum
	{
		ACTION_F_LOOK_LR,
		ACTION_F_LOOK_UD,

		ACTION_F_WALK_FB,
		ACTION_F_WALK_LR,

		ACTION_B_FIRE,
		ACTION_B_JUMP,

		ACTION_B_WALK_LEFT,
		ACTION_B_WALK_RIGHT,
		ACTION_B_WALK_FORWARD,
		ACTION_B_WALK_BACKWARD,

		ACTION_B_LOOK_UP,
		ACTION_B_LOOK_DOWN,
		ACTION_B_LOOK_LEFT,
		ACTION_B_LOOK_RIGHT,

		ACTION_B_KILL_PLAYER,
		ACTION_B_SPRINT,

		ACTION_B_CHANGE_AMMUNITIONTYPE,
		ACTION_B_CHANGE_FIRINGMODE,

		ACTION_B_AMMUNITION_BULLET,
		ACTION_B_AMMUNITION_SCATTER,
		ACTION_B_AMMUNITION_EXPLOSIVE,

		ACTION_B_FIRINGMODE_SINGLE,
		ACTION_B_FIRINGMODE_SEMI,
		ACTION_B_FIRINGMODE_AUTO,

		ACTION_B_RUMBLE_ON,
		ACTION_B_RUMBLE_OFF,
		ACTION_B_TOGGLE_MUTE_SOUND,

		ACTION_LAST
	};

	//For enum to string conversion, there might be a better way
	std::string InputActionStrings[ACTION_LAST];

private:
	void initializeStringArray()
	{
		InputActionStrings[ACTION_F_LOOK_LR]				=	"Look Left/Right [Axis]";
		InputActionStrings[ACTION_F_LOOK_UD]				=	"Look Up/Down [Axis]";
		InputActionStrings[ACTION_F_WALK_FB]				=	"Walk [Axis]";
		InputActionStrings[ACTION_F_WALK_LR]				=	"Strafe [Axis]";
		InputActionStrings[ACTION_B_FIRE]					=	"Fire";
		InputActionStrings[ACTION_B_CHANGE_AMMUNITIONTYPE]	=	"Next Ammunition";
		InputActionStrings[ACTION_B_CHANGE_FIRINGMODE]		=	"Next Firing Mode";
		InputActionStrings[ACTION_B_JUMP]					=	"Jump";
		InputActionStrings[ACTION_B_WALK_LEFT]				=	"Strafe Left";
		InputActionStrings[ACTION_B_WALK_RIGHT]				=	"Strafe Right";
		InputActionStrings[ACTION_B_WALK_FORWARD]			=	"Walk Forward";
		InputActionStrings[ACTION_B_WALK_BACKWARD]			=	"Walk Backward";
		InputActionStrings[ACTION_B_LOOK_UP]				=	"Look Up";
		InputActionStrings[ACTION_B_LOOK_DOWN]				=	"Look Down";
		InputActionStrings[ACTION_B_LOOK_LEFT]				=	"Look Left";
		InputActionStrings[ACTION_B_LOOK_RIGHT]				=	"Look Right";
		InputActionStrings[ACTION_B_KILL_PLAYER]			=	"Suicide";
		InputActionStrings[ACTION_B_SPRINT]					=	"Sprint";
		InputActionStrings[ACTION_B_FIRINGMODE_SINGLE]		=	"Select Single";
		InputActionStrings[ACTION_B_FIRINGMODE_SEMI]		=	"Select Semi";
		InputActionStrings[ACTION_B_FIRINGMODE_AUTO]		=	"Select Auto";
		InputActionStrings[ACTION_B_AMMUNITION_BULLET]		=	"Select Bullet";
		InputActionStrings[ACTION_B_AMMUNITION_SCATTER]		=	"Select Scatter";
		InputActionStrings[ACTION_B_AMMUNITION_EXPLOSIVE]	=	"Select Explosive";
		InputActionStrings[ACTION_B_RUMBLE_ON]				=	"Turn Rumble On";
		InputActionStrings[ACTION_B_RUMBLE_OFF]				=	"Turn Rumble Off";
		InputActionStrings[ACTION_B_TOGGLE_MUTE_SOUND]		=	"Mute Sound";
	}
};