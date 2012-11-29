#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0) : QMainWindow(parent, flags)
	{
		// Create UI generated from XML file
		ui.setupUi(this);
		QString ss;
		this->setWindowTitle("XKILL");
		resize(800, 600);
	}
	~MainWindow()
	{
	}

private:
	Ui::MainWindowClass ui;
	QWidget* gameWidget;
	
};

#endif // MAINWINDOW_H
