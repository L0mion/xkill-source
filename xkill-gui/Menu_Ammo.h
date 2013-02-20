#pragma once

#include "ui_MainMenu2.h"

#include <xkill-utilities/MutatorSettings.h>

//! A class to handle interaction between the tab "Ammunition" and the game

class Menu_Ammo : QObject
{
	Q_OBJECT

public:
	Menu_Ammo(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Ammo();

public slots:
	//! Updates the qt menu based on the in game data
	void setSettingsMenu();
	//! Updates the in game data based on the qt menu
	void settingsMenuUpdated();

private:
	//! A local pointer to the qt menu
	Ui::MainWindow* ui;
	
	// Lock to prevent menu from updating if we are setting values
	bool updateLock;

	//! Used to change in game standard values
	MutatorSettings* mutatorSettings_;

	//! Gets the ammunition that is to be changed or read data from
	Ammunition* getAmmoSettings();
};