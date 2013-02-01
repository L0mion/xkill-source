#pragma once

#include "ui_MainMenu.h"

class Menu_Sound
{
public:
	Menu_Sound(Ui::MainMenu* ui, QMainWindow* window);
	~Menu_Sound();

	void updateMenu();

private:
	Ui::MainMenu* ui;
};