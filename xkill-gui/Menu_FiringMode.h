#pragma once

#include "ui_MainMenu.h"

#include <xkill-utilities/MutatorSettings.h>

//! A class to handle interaction between the tab "Weapon" and the game

class Menu_FiringMode
{
public:
	Menu_FiringMode(Ui::MainMenu* ui);
	~Menu_FiringMode();

	//! Updates the qt menu based on the in game data
	void setSettingsMenu();
	//! Updates the in game data based on the qt menu
	void settingsMenuUpdated();

private:
	//! A local pointer to the qt menu
	Ui::MainMenu* ui;
	//! Used to change in game standard values
	MutatorSettings* mutatorSettings_;

	//! Gets the firingMode that is to be changed or read data from
	FiringMode* getFiringModeSettings();
};