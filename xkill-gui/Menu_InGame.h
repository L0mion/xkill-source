#pragma once

#include <QtGui/QMainWindow>
#include "ToggleHelper.h"
#include "ui_InGameMenu.h"

class Menu_InGame : public QMainWindow, public ToggleHelper
{
	Q_OBJECT

private:
	Ui::InGameMenu ui;

public:
	Menu_InGame(QWidget* parent);
	void parentMoveEvent();

public slots:
	void slot_return();
	void slot_quitToMenu();
	void slot_quitToDesktop();
};