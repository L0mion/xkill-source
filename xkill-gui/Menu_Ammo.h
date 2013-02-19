#pragma once

#include "ui_MainMenu2.h"

#include <xkill-utilities/MutatorSettings.h>

//! A class to handle interaction between the tab "Ammunition" and the game

class Menu_Ammo
{
public:
	Menu_Ammo(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Ammo();

	//! Updates the qt menu based on the in game data
	void setSettingsMenu();
	//! Updates the in game data based on the qt menu
	void settingsMenuUpdated();

private:
	//! A local pointer to the qt menu
	Ui::MainWindow* ui;
	//! Used to change in game standard values
	MutatorSettings* mutatorSettings_;

	//! Gets the ammunition that is to be changed or read data from
	Ammunition* getAmmoSettings();
};