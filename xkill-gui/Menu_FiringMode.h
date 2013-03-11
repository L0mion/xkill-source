#pragma once

#include "ui_MainMenu.h"

#include <xkill-utilities/MutatorSettings.h>

//! A class to handle interaction between the tab "Weapon" and the game

class Menu_FiringMode : QObject
{
	Q_OBJECT

public:
	Menu_FiringMode(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_FiringMode();

private:
	//! A local pointer to the qt menu
	Ui::MainWindow* ui;

	// Lock to prevent menu from updating if we are setting values
	bool updateLock;

	//! Used to change in game standard values
	MutatorSettings* mutatorSettings_;


	//! Gets the firingMode that is to be changed or read data from
	FiringMode* getFiringModeSettings();

public slots:
	//! Reset to standard values
	void reset();

	//! Updates the qt menu based on the in game data
	void setSettingsMenu();

	//! Updates the in game data based on the qt menu
	void settingsMenuUpdated();

};