#pragma once

#include <xkill-utilities/Util.h>
#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
#include "ToggleHelper.h"
#include "ui_ScoreMenu.h" 

class Menu_ScoreBoard : public QMainWindow, public ToggleHelper
{
private:
	Ui::ScoreMenu ui;

public:
	Menu_ScoreBoard(QWidget* parent = 0, Qt::WFlags flags = 0);

	// Not working! What's wrong with Qt?
	void mapListToAttributes(int numAttributes);

	void onUpdate(float delta);

	void parentMoveEvent();

protected:
	void keyPressEvent(QKeyEvent* e);
};