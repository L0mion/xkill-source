#pragma once

#include "ui_MainMenu2.h"

class Menu_Sound : QObject
{
	Q_OBJECT
public:
	Menu_Sound(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Sound();

public slots:
	void settingsMenuUpdated();

private:
	Ui::MainWindow* ui;
};