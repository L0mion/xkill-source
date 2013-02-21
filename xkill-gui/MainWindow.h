#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QFocusEvent>
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
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void moveEvent(QMoveEvent *e);
	void resizeEvent(QResizeEvent* e);;
	void showMenu();
	


public slots:
	void slot_setTitle(QString title);
	void slot_toggleFullScreen();
};

#endif // MAINWINDOW_H
