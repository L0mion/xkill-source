#pragma once

#include <QtGui/QMainWindow>
#include "ToggleHelper.h"
#include "ui_MainMenu2.h"

class Menu_Main2 : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow ui;

public:
	Menu_Main2( QWidget* parent ) : QMainWindow(parent)
	{
		ui.setupUi(this);
		setWindowFlags(Qt::FramelessWindowHint);
		setAttribute(Qt::WA_TranslucentBackground);
		QWidget::show();
	}
	~Menu_Main2();
};