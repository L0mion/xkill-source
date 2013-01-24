#pragma once

#include "ui_MainMenu.h"

#include <xkill-utilities/MutatorSettings.h>

class Menu_Ammo
{
public:
	Menu_Ammo(Ui::MainMenu* ui);
	~Menu_Ammo();

	void setSettingsMenu();
	void settingsMenuUpdated();

private:
	Ui::MainMenu* ui;
	MutatorSettings* mutatorSettings_;

	Ammunition* getAmmoSettings();
};