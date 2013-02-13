#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/qevent.h>
#include <QtGui/QMouseEvent> // needed to grab mouse input
#include <QtGui/QMessageBox> // used to display info dialogs
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/Util.h>
#include <QtGui/QApplication>
#include "GameWidget.h"
#include "MenuManager.h"
#include "Menu_Editor.h"
#include "Menu_Main2.h"


class MainWindow : public QMainWindow, public IObserver
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	MenuManager* menuManager;
	GameWidget* gameWidget;
	Menu_Main2* menu;
	
public:
	MainWindow();
	~MainWindow();

	void onUpdate(float delta);
	void onEvent(Event* e);
	void event_showMessageBox(Event_ShowMessageBox* e);

protected:
	void closeEvent(QCloseEvent *event);
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);
	void moveEvent(QMoveEvent *e);
	void resizeEvent(QResizeEvent* e);;
	void showMenu();
	void keyReleaseEvent(QKeyEvent* e);

	bool event(QEvent* e)
	{
		// A custom event filter

		// Perform logic as normal
		bool result = QMainWindow::event(e) ;

		// Make sure menu is always drawn on top of this window
		// event though it is a separate window
		switch(e->type())
		{
		case QEvent::WindowActivate :
			menu->raise();
			break;
		};

		return result;
	}
	void paintEvent(QPaintEvent* e);

public slots:
	void slot_setTitle(QString title);
	void slot_toggleFullScreen();
};

#endif // MAINWINDOW_H
