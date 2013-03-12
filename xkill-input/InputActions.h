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
		ACTION_B_FIRE,
		ACTION_B_RELOAD,
		ACTION_B_LOW_SENSITIVITY,
		ACTION_B_JUMP_JETPACK,
		ACTION_B_SPRINT,

		ACTION_B_NEXT_AMMUNITIONTYPE,
		ACTION_B_PREV_AMMUNITIONTYPE,
		ACTION_B_NEXT_FIRINGMODE,
		ACTION_B_PREV_FIRINGMODE,

		ACTION_F_LOOK_LR,
		ACTION_F_LOOK_UD,
		ACTION_F_WALK_FB,
		ACTION_F_WALK_LR,

		ACTION_B_WALK_LEFT,
		ACTION_B_WALK_RIGHT,
		ACTION_B_WALK_FORWARD,
		ACTION_B_WALK_BACKWARD,

		ACTION_B_LOOK_UP,
		ACTION_B_LOOK_DOWN,
		ACTION_B_LOOK_LEFT,
		ACTION_B_LOOK_RIGHT,

		ACTION_B_AMMUNITION_BULLET,
		ACTION_B_AMMUNITION_SCATTER,
		ACTION_B_AMMUNITION_EXPLOSIVE,

		ACTION_B_FIRINGMODE_SINGLE,
		ACTION_B_FIRINGMODE_SEMI,
		ACTION_B_FIRINGMODE_AUTO,

		//Temporary debug inputs

		ACTION_B_KILL_PLAYER,

		ACTION_B_RUMBLE_ON,
		ACTION_B_RUMBLE_OFF,

		ACTION_B_TIME_SPEED_DOWN,
		ACTION_B_TIME_SPEED_UP,

		ACTION_LAST
	};

	//For enum to string conversion, there might be a better way
	std::string InputActionStrings[ACTION_LAST];

private:
	void initializeStringArray()
	{
		InputActionStrings[ACTION_F_LOOK_LR					] = "Look Left/Right [Axis]";
		InputActionStrings[ACTION_F_LOOK_UD					] = "Look Up/Down [Axis]";
		InputActionStrings[ACTION_F_WALK_FB					] = "Walk [Axis]";
		InputActionStrings[ACTION_F_WALK_LR					] = "Strafe [Axis]";
		InputActionStrings[ACTION_B_FIRE					] = "Fire";
		InputActionStrings[ACTION_B_JUMP_JETPACK			] = "Jump/Jetpack";
		InputActionStrings[ACTION_B_WALK_LEFT				] = "Strafe Left";
		InputActionStrings[ACTION_B_WALK_RIGHT				] = "Strafe Right";
		InputActionStrings[ACTION_B_WALK_FORWARD			] = "Walk Forward";
		InputActionStrings[ACTION_B_WALK_BACKWARD			] = "Walk Backward";
		InputActionStrings[ACTION_B_LOOK_UP					] = "Look Up";
		InputActionStrings[ACTION_B_LOOK_DOWN				] = "Look Down";
		InputActionStrings[ACTION_B_LOOK_LEFT				] = "Look Left";
		InputActionStrings[ACTION_B_LOOK_RIGHT				] = "Look Right";
		InputActionStrings[ACTION_B_KILL_PLAYER				] = "Suicide";
		InputActionStrings[ACTION_B_SPRINT					] = "Sprint";
		InputActionStrings[ACTION_B_NEXT_AMMUNITIONTYPE		] = "Next Ammo";
		InputActionStrings[ACTION_B_PREV_AMMUNITIONTYPE		] = "Prev Ammo";
		InputActionStrings[ACTION_B_NEXT_FIRINGMODE			] = "Next FiringMode";
		InputActionStrings[ACTION_B_PREV_FIRINGMODE			] = "Prev FiringMode";
		InputActionStrings[ACTION_B_AMMUNITION_BULLET		] = "Ammunition Bullet";
		InputActionStrings[ACTION_B_AMMUNITION_SCATTER		] = "Ammunition Scatter";
		InputActionStrings[ACTION_B_AMMUNITION_EXPLOSIVE	] = "Ammunition Explosive";
		InputActionStrings[ACTION_B_FIRINGMODE_SINGLE		] = "FiringMode Single";
		InputActionStrings[ACTION_B_FIRINGMODE_SEMI			] = "FiringMode Semi";
		InputActionStrings[ACTION_B_FIRINGMODE_AUTO			] = "FiringMode Auto";
		InputActionStrings[ACTION_B_RUMBLE_ON				] = "Rumble On";
		InputActionStrings[ACTION_B_RUMBLE_OFF				] = "Rumble Off";
		InputActionStrings[ACTION_B_TIME_SPEED_DOWN			] = "Slow Down Time";
		InputActionStrings[ACTION_B_TIME_SPEED_UP			] = "Speed Up Time";
		InputActionStrings[ACTION_B_RELOAD					] = "Reload";
		InputActionStrings[ACTION_B_LOW_SENSITIVITY			] = "Aim";
	}
};