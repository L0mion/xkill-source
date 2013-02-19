#pragma once

#include "ui_MainMenu2.h"

class Menu_Sound
{
public:
	Menu_Sound(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Sound();

	void updateMenu();

private:
	Ui::MainWindow* ui;
};