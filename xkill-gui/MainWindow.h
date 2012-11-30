#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <xkill-utilities/IObserver.h>
#include "ui_MainWindow.h"

#include "GameWidget.h"

class MainWindow : public QMainWindow, public IObserver
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

		// init console
		AllocConsole();

		// init gameWidget
		gameWidget = new GameWidget(this);

		SetStdHandle(STD_INPUT_HANDLE |STD_OUTPUT_HANDLE, this->winId());

		// show fps-counter on title bar
		connect(gameWidget, SIGNAL(signal_fpsChanged(QString)), this, SLOT(slot_setTitle(QString)));
	}
	~MainWindow()
	{
		delete gameWidget;
	}

	void onUpdate(float delta)
	{
	}

	void onEvent(Event* e)
	{
	}

public slots:
	void slot_setTitle(QString title)
	{
		MainWindow::setWindowTitle("XKILL  [" + title + "]");
	};
};

#endif // MAINWINDOW_H
