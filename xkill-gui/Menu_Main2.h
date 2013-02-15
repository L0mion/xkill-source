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
		
		// Center background 
		ui.label_background->move(0,0);

		// Hide unused menus
		ui.frame_main->hide();
		ui.frame_start->hide();
		ui.frame_customize->hide();
		ui.frame_settings->hide();
		ui.frame_video->hide();
		ui.frame_audio->hide();
		ui.frame_input->hide();
		
		// Show main menu
		push_menu(ui.frame_main);

		// Setup buttons
		connect(ui.pushButton_start, SIGNAL(clicked()),	this, SLOT(slot_menu_start()));
		connect(ui.pushButton_customize, SIGNAL(clicked()),	this, SLOT(slot_menu_customize()));
		connect(ui.pushButton_settings, SIGNAL(clicked()),	this, SLOT(slot_menu_settings()));
		connect(ui.pushButton_video, SIGNAL(clicked()),	this, SLOT(slot_menu_video()));
		connect(ui.pushButton_audio, SIGNAL(clicked()),	this, SLOT(slot_menu_audio()));
		connect(ui.pushButton_input, SIGNAL(clicked()),	this, SLOT(slot_menu_input()));
		connect(ui.pushButton_quit, SIGNAL(clicked()),	this, SLOT(slot_menu_quit()));
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
		// Hide previous menu, if any
		if(menuStack.size() > 0)
		{
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
		}

		// Show new menu
		menu->move(0,0);
		menu->resize(width(), height());
		menu->show();
		menuStack.push_back(menu);
	}

	void pop_menu()
	{
		// Make sure rot-menu is not poped
		if(menuStack.size() > 1)
		{
			// Pop current menu
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
			menuStack.pop_back();

			// Show previous menu
			QFrame* menu = menuStack.back();
			menu->move(0,0);
			menu->resize(width(), height());
			menu->show();
		}
	}

	void menuResize()
	{
		QFrame* topMenu = menuStack.back();

		// Resize background
		ui.label_background->resize(width(), height());
		ui.label_background->lower();

		// Resize current menu
		topMenu->resize(width(), height());
		//topMenu->resize(ui,->minimumSize());
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

protected:
	void mousePressEvent(QMouseEvent *e)
	{
		if(e->button() == Qt::RightButton)
			pop_menu();
	}

private slots:
	void slot_menu_main()
	{
		push_menu(ui.frame_main);
	}
	void slot_menu_start()
	{
		push_menu(ui.frame_start);
	}
	void slot_menu_settings()
	{
		push_menu(ui.frame_settings);
	}
	void slot_menu_video()
	{
		push_menu(ui.frame_video);
	}
	void slot_menu_audio()
	{
		push_menu(ui.frame_audio);
	}
	void slot_menu_customize()
	{
		push_menu(ui.frame_customize);
	}
	void slot_menu_input()
	{
		push_menu(ui.frame_input);
	}
	void slot_menu_quit()
	{
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
	}
};