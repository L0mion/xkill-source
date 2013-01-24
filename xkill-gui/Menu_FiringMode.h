#pragma once

#include "ui_MainMenu.h"

#include <xkill-utilities/MutatorSettings.h>

class Menu_FiringMode
{
public:
	Menu_FiringMode(Ui::MainMenu* ui);
	~Menu_FiringMode();

	void setSettingsMenu();
	void settingsMenuUpdated();

private:
	Ui::MainMenu* ui;
	MutatorSettings* mutatorSettings_;

	FiringMode* getFiringModeSettings();
};