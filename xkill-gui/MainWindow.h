#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

#include "GameWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	QWidget* gameWidget;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0) : QMainWindow(parent, flags)
	{
		// create UI generated from XML file
		ui.setupUi(this);
		MainWindow::setWindowTitle("XKILL");
		resize(800, 600);

		// init gameWidget
		gameWidget = new GameWidget(this);

		// show fps-counter on title bar
		connect(gameWidget, SIGNAL(signal_fpsChanged(QString)), this, SLOT(slot_setTitle(QString)));
	}
	~MainWindow()
	{
		delete gameWidget;
	}

public slots:
	void slot_setTitle(QString title)
	{
		MainWindow::setWindowTitle("XKILL  [" + title + "]");
	};
};

#endif // MAINWINDOW_H
