#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <xkill-utilities/Util.h>
#include "ui_MainMenu2.h"
#include <vector>

class Menu_Main2 : public QMainWindow, IObserver
{
	Q_OBJECT

private:
	Ui::MainWindow ui;
	QWidget* parent;
	std::vector<QFrame*> menuStack; // Used as state machine to return to the previous menu

public:
	Menu_Main2(QWidget* parent) : QMainWindow()
	{
		this->parent = parent;
		ui.setupUi(this);
		QWidget::setWindowFlags(Qt::FramelessWindowHint);
		alwaysOnTop(true);
		QWidget::setAttribute(Qt::WA_TranslucentBackground);
		QWidget::show();
		
		SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
		SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);

		ui.frame_main->move(0,0);
		ui.frame_start->move(0,0);
		ui.frame_settings->move(0,0);
		
		ui.frame_main->hide();
		ui.frame_start->hide();
		ui.frame_settings->hide();

		push_menu(ui.frame_main);
	}
	~Menu_Main2()
	{
		UNSUBSCRIBE_TO_EVENTS(this);
	}
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_WINDOW_MOVE:
			event_windowMove((Event_WindowMove*)e);
			break;
		case EVENT_WINDOW_RESIZE:
			event_windowResize((Event_WindowResize*)e);
			break;
		default:
			break;
		}
	}

	void push_menu(QFrame* menu)
	{
		// Hide previous menu if any
		if(menuStack.size() > 0)
		{
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
		}

		// Show new menu
		ui.frame_main->resize(ui.label_background->width(), ui.label_background->height());
		menu->show();
		menuStack.push_back(menu);
	}

	void pop_menu()
	{
		QFrame* topMenu = menuStack.back();
		topMenu->hide();
		menuStack.pop_back();
	}

	void menuResize()
	{
		QFrame* topMenu = menuStack.back();

		// Resize background
		ui.label_background->resize(width(), height());
		ui.label_background->lower();

		// Resize current menu
		topMenu->resize(width(), height());
	}


	void event_windowMove(Event_WindowMove* e)
	{
		move(e->pos.x, e->pos.y);
	}

	void alwaysOnTop(bool on)
	{
		if(on)
		{
			// Enable Window Stay on Top flag
			this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
		}
		else
		{
			// Disable Window Stay on Top flag
			this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
		}
	}


	void event_windowResize(Event_WindowResize* e)
	{
		// Resize window
		resize(e->width, e->height);

		// Make sure menu is resized as well
		menuResize();
	}

	void keyPressEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}
	void keyReleaseEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}

	void closeEvent(QCloseEvent* event)
	{
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
	}
};