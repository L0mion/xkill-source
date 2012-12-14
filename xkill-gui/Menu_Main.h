#pragma once

#include <QMainWindow>
#include "ToggleHelper.h"
#include "ui_MainMenu.h" 

class Menu_Main : public QMainWindow, public ToggleHelper
{
private:
	Ui::MainMenu ui;

public:
	Menu_Main(QWidget* parent) : QMainWindow(parent), ToggleHelper(this)
	{
		ui.setupUi(this);
		QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
		QWidget::setWindowFlags(Qt::SplashScreen);

		// Failed attempts at making transparent window
		//this->setWindowOpacity(false);
		//QWidget::setWindowFlags(Qt::FramelessWindowHint);
		//this->setStyleSheet("QFrame{background-color: white;}");
		//this->setStyleSheet("QWidget{background-color: black;}");
		connect(ui.pushButton_Exit, SIGNAL(clicked()), parentWidget(), SLOT(close()));
		connect(ui.pushButton_FullScreen, SIGNAL(toggled(bool)), parentWidget(), SLOT(toggleFullScreen(bool)));
	}

	void parentMoveEvent()
	{
		QPoint pos = parentWidget()->pos();
		int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
		int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
		move(x, y);
	}

protected:
	void keyPressEvent(QKeyEvent* e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
};